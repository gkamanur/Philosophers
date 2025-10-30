/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:56:50 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/30 11:40:55 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

pthread_mutex_t	*get_first_fork(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
		return (philo->left_fork);
	return (philo->right_fork);
}

pthread_mutex_t	*get_second_fork(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
		return (philo->right_fork);
	return (philo->left_fork);
}

void	take_forks(t_philo *philo, pthread_mutex_t *first,
	pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_message(philo, "has taken a fork");
}

void	eat_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->data->meal_lock);
	print_message(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data->meal_lock);
}

void	release_forks(pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}
