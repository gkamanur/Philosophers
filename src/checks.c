/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:50:02 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/30 11:39:53 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_death(t_philo *philo)
{
	long	current_time;
	long	time_since_meal;

	pthread_mutex_lock(&philo->data->meal_lock);
	current_time = get_current_time();
	time_since_meal = current_time - philo->last_meal_time;
	pthread_mutex_unlock(&philo->data->meal_lock);
	if (time_since_meal >= philo->data->time_to_die)
		return (1);
	return (0);
}

int	is_dead(t_data *data)
{
	int	dead;

	pthread_mutex_lock(&data->death_lock);
	dead = data->dead;
	pthread_mutex_unlock(&data->death_lock);
	return (dead);
}

int	is_finished(t_data *data)
{
	int	finished;

	pthread_mutex_lock(&data->death_lock);
	finished = data->finished;
	pthread_mutex_unlock(&data->death_lock);
	return (finished);
}

void	print_death_message(t_philo *philo)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->write_lock);
	timestamp = get_current_time() - philo->data->start_time;
	printf("%ld %d died\n", timestamp, philo->id);
	pthread_mutex_unlock(&philo->data->write_lock);
}
