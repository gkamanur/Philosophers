# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview

This is a **Dining Philosophers** simulation solving the classic concurrency problem using POSIX threads and mutexes. The project demonstrates advanced thread synchronization, deadlock prevention, and race condition handling.

**Critical constraint**: The simulation must detect philosopher death within 10ms of occurrence, requiring precise timing and continuous monitoring.

## Build Commands

```bash
make           # Compile the philo binary
make clean     # Remove object files
make fclean    # Remove all generated files (objects + binary)
make re        # Full recompile (fclean + all)
```

**Compilation flags**: `-Wall -Wextra -Werror -pthread`

## Testing

### Quick Validation
```bash
./QUICK_TEST.sh
```
Runs 9 critical test cases including mandatory evaluation tests, edge cases, and stress tests (up to 199 philosophers).

### Manual Testing

**Mandatory evaluation tests**:
```bash
./philo 1 800 200 200          # Should die at ~800ms
./philo 5 800 200 200 7        # Should complete without deaths
./philo 4 410 200 200 10       # Should complete without deaths
./philo 4 310 200 100 10       # Should die around 310ms
```

**Stress tests**:
```bash
./philo 131 605 200 200 10     # Large count, tight timing
./philo 199 610 200 200 10     # Maximum tested count
```

**Usage**: `./philo nb_philos time_to_die time_to_eat time_to_sleep [nb_meals]`
- All times in milliseconds
- `nb_meals` is optional (omit for infinite simulation)

### External Test Suite
The `42-philosophers-tester/` directory may contain additional tests.

## Architecture

### Core Components

**Main flow** (`main.c`):
1. Parse arguments → allocate resources → initialize mutexes
2. Initialize philosophers with circular fork assignment
3. Create philosopher threads → monitor for death/completion
4. Join threads → cleanup resources

**Philosopher lifecycle** (`routine.c`):
- Each philosopher runs in a separate thread
- Cycle: think → eat → sleep (repeat until death or completion)
- Even-numbered philosophers stagger their start to reduce initial contention

**Monitor thread** (`monitor_death.c`):
- Runs in the main thread continuously (no sleep delays)
- Checks each philosopher for death every iteration
- Verifies if all philosophers completed required meals

### Critical Synchronization Details

#### Deadlock Prevention
**Ordered fork acquisition** (`routine.c:start_eating()`):
- Forks are always locked in consistent memory address order
- Prevents circular wait condition regardless of philosopher count
- Works reliably with 1-199+ philosophers

```c
// Always lock lower address fork first
if (philo->left_fork < philo->right_fork)
    lock(left_fork), lock(right_fork)
else
    lock(right_fork), lock(left_fork)
```

#### Race Condition Prevention
**Atomic meal counting** (`routine.c`, `monitor_death.c`):
- `meal_lock` protects all meal count operations
- Lock held during entire "check all ate" operation
- Prevents inconsistent state between eating and checking

**State synchronization**:
- `death_lock`: Protects `data->dead` flag
- `write_lock`: Protects console output (prevents interleaved messages)
- `meal_lock`: Protects meal counts and last meal timestamps

#### Starvation Prevention
**Adaptive start staggering** (`routine.c:philosopher_routine()`):
- 100+ philosophers: 50ms stagger for even IDs
- 50-99: 30ms stagger
- 11-49: 15ms stagger
- 4-10: 1ms stagger
- 2-3: No stagger

**Thinking delays for odd counts** (`routine.c:start_sleeping_and_thinking()`):
- Small odd counts (3-9) add 1-5ms thinking time
- Prevents starvation when philosophers can't synchronize naturally
- More thinking needed for tight time constraints

#### Timing Precision
**Adaptive sleep** (`utils.c:ft_usleep()`):
- Sleeps longer when far from target time
- Sleeps in smaller increments when close
- Prevents timing drift while maintaining efficiency

### Data Structures

**t_data** (shared state):
- Simulation parameters: `nb_philo`, `time_to_die`, `time_to_eat`, `time_to_sleep`, `nb_meals`
- Synchronization: `death_lock`, `write_lock`, `meal_lock`
- State flags: `dead`, `finished`
- Arrays: `forks[]`, `philos[]`

**t_philo** (per-philosopher state):
- Identity: `id`
- State: `meals_count`, `last_meal_time`
- Resources: `left_fork`, `right_fork`, `thread`
- Reference to shared data: `data`

### Edge Cases

**Single philosopher** (`routine.c:handle_single_philo()`):
- Takes one fork and waits for death
- Cannot eat (needs two forks)
- Must properly detect death at `time_to_die`

**Two philosophers**:
- No stagger needed
- Simple alternating pattern works

**Large counts (131+)**:
- Ordered locking critical to prevent deadlock
- Large stagger (50ms) prevents thundering herd
- Continuous monitoring essential for fast death detection

## Key Implementation Notes

1. **Death detection must be immediate**: Monitor loop has no delays, checks continuously
2. **All state access must be synchronized**: Use appropriate locks for shared data
3. **Fork order matters**: Always lock in address order to prevent deadlock
4. **Start timing matters**: Stagger prevents all philosophers grabbing one fork simultaneously
5. **Message printing must check both dead AND finished states**: Prevents output after simulation end

## File Structure

- `main.c`: Entry point, initialization, simulation orchestration
- `init.c`: Argument parsing, resource allocation, mutex initialization
- `routine.c`: Philosopher thread logic, eating/sleeping/thinking cycle
- `monitor_death.c`: Death detection and completion checking
- `checks.c`: Thread-safe state checkers (`is_dead`, `is_finished`, `check_death`)
- `utils.c`: Timing utilities, message printing
- `cleanup.c`: Resource deallocation, mutex destruction
- `ft_atoi.c`: Custom atoi implementation
- `philo.h`: Type definitions and function prototypes

## Common Issues to Avoid

1. **Locking forks in inconsistent order**: Causes deadlock with many philosophers
2. **Not checking `finished` flag in print_message**: Prints after simulation should end
3. **Sleeping in monitor loop**: Misses deaths or detects them too late
4. **Individual locks during "all ate" check**: Race condition where meals can complete between checks
5. **Insufficient start stagger**: Thundering herd problem with large philosopher counts
6. **Fixed sleep chunks in ft_usleep**: Causes timing drift and imprecision
