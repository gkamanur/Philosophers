/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:50:30 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/30 11:06:44 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	int					meals_count;
	long				last_meal_time;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					nb_philo;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					nb_meals;
	long				start_time;
	int					dead;
	int					finished;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		death_lock;
	pthread_mutex_t		meal_lock;
	t_philo				*philos;
}						t_data;

/* Utils */
int						ft_atoi(const char *str);
long					get_current_time(void);
void					ft_usleep(long time);
void					print_message(t_philo *philo, char *message);
void					print_death_message(t_philo *philo);

/* Initialization */
int						parse_args(t_data *data, int argc, char **argv);
int						init_mutexes(t_data *data);
int						alloc_resources(t_data *data);
void					init_forks(t_data *data);

int						init_data(t_data *data, int argc, char **argv);
int						init_philos(t_data *data);

/* eating */
void					release_forks(pthread_mutex_t *first,
							pthread_mutex_t *second);
void					eat_meal(t_philo *philo);
void					take_forks(t_philo *philo, pthread_mutex_t *first,
							pthread_mutex_t *second);
pthread_mutex_t			*get_second_fork(t_philo *philo);
pthread_mutex_t			*get_first_fork(t_philo *philo);

/* Stagger */
void					apply_stagger_if_needed(t_philo *philo);
long					calculate_stagger(t_philo *philo);

/* Simulation */
void					*philosopher_routine(void *arg);
int						start_simulation(t_data *data);
void					monitor_death(t_data *data);

/* Cleanup */
void					cleanup(t_data *data);

/* Checks */
int						check_death(t_philo *philo);
int						is_dead(t_data *data);
int						is_finished(t_data *data);

#endif