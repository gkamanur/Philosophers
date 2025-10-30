#!/bin/bash

# Quick validation script for philosophers project

echo "🧪 PHILOSOPHERS - QUICK VALIDATION"
echo "===================================="
echo

# Check if binary exists
if [ ! -f "./philo" ]; then
    echo "❌ Binary not found. Compiling..."
    make
    echo
fi

pass=0
fail=0

run_test() {
    local name="$1"
    shift
    printf "%-50s" "$name"
    
    if timeout 10 ./philo "$@" > /dev/null 2>&1; then
        echo "✅ PASS"
        ((pass++))
    else
        echo "❌ FAIL"
        ((fail++))
    fi
}

run_test_should_die() {
    local name="$1"
    shift
    printf "%-50s" "$name"
    
    output=$(timeout 10 ./philo "$@" 2>&1)
    if echo "$output" | grep -q "died"; then
        echo "✅ PASS (died as expected)"
        ((pass++))
    else
        echo "❌ FAIL (should have died)"
        ((fail++))
    fi
}

echo "=== MANDATORY TESTS ==="
run_test_should_die "1. Single philosopher (must die)" 1 800 200 200
run_test "2. Five philosophers, 7 meals" 5 800 200 200 7
run_test "3. Four philosophers, tight timing" 4 410 200 200 10
run_test_should_die "4. Four philosophers (must die)" 4 310 200 100 10

echo
echo "=== CRITICAL EDGE CASES ==="
run_test "5. Two philosophers" 2 410 200 200 10
run_test "6. Three philosophers" 3 610 200 200 10
run_test "7. Five philosophers" 5 610 200 200 10

echo
echo "=== STRESS TESTS ==="
run_test "8. 131 philosophers" 131 605 200 200 10
run_test "9. 199 philosophers" 199 610 200 200 10

echo
echo "===================================="
echo "RESULTS: $pass passed, $fail failed"
echo "===================================="

if [ $fail -eq 0 ]; then
    echo "✅ ALL TESTS PASSED - Ready for evaluation!"
    exit 0
else
    echo "❌ Some tests failed - needs fixes"
    exit 1
fi
