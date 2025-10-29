/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 13:33:44 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/29 11:37:28 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_single_philo(t_philo *philo)
{
	print_message(philo, "has taken a fork");
	// Wait for death to be detected
	while (!is_dead(philo->data))
		ft_usleep(1);
}

static void	start_eating(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	// Lock forks in consistent order to prevent deadlock
	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->data->meal_lock);
	print_message(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data->meal_lock);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

static void	start_sleeping_and_thinking(t_philo *philo)
{
	print_message(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	print_message(philo, "is thinking");
	// Add minimal thinking for small odd counts to prevent starvation
	if (philo->data->nb_philo % 2 != 0 && philo->data->nb_philo < 10 && philo->data->nb_philo > 1)
	{
		// More thinking time needed for tighter time constraints
		if (philo->data->time_to_die < philo->data->time_to_eat * philo->data->nb_philo)
			ft_usleep(5);
		else
			ft_usleep(1);
	}
}

static void	philosopher_cycle(t_philo *philo)
{
	while (!is_dead(philo->data) && !is_finished(philo->data))
	{
		if (philo->data->nb_philo == 1)
		{
			handle_single_philo(philo);
			break ;
		}
		start_eating(philo);
		start_sleeping_and_thinking(philo);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// Stagger even philosophers to reduce initial contention
	if (philo->id % 2 == 0)
	{
		if (philo->data->nb_philo > 100)
			ft_usleep(50);
		else if (philo->data->nb_philo > 50)
			ft_usleep(30);
		else if (philo->data->nb_philo > 10)
			ft_usleep(15);
		else if (philo->data->nb_philo > 3)
			ft_usleep(1);
	}
	philosopher_cycle(philo);
	return (NULL);
}
