/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_death.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 13:35:40 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/30 13:27:10 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	handle_philo_death(t_data *data, t_philo *philo)
{
	if (check_death(philo))
	{
		print_death_message(philo);
		pthread_mutex_lock(&data->death_lock);
		data->dead = 1;
		pthread_mutex_unlock(&data->death_lock);
		return (1);
	}
	return (0);
}

static int	check_all_ate(t_data *data)
{
	int	i;
	int	all_ate;

	if (data->nb_meals == -1)
		return (0);
	i = 0;
	all_ate = 1;
	pthread_mutex_lock(&data->meal_lock);
	while (i < data->nb_philo)
	{
		if (data->philos[i].meals_count < data->nb_meals)
		{
			all_ate = 0;
			break ;
		}
		i++;
	}
	if (all_ate)
		printf("All philosophers have eaten %d times.\n", data->nb_meals);
	pthread_mutex_unlock(&data->meal_lock);
	return (all_ate);
}

void	monitor_death(t_data *data)
{
	int	i;

	while (!is_dead(data))
	{
		i = 0;
		while (i < data->nb_philo)
		{
			if (handle_philo_death(data, &data->philos[i]))
				return ;
			i++;
		}
		if (check_all_ate(data))
		{
			pthread_mutex_lock(&data->death_lock);
			data->finished = 1;
			pthread_mutex_unlock(&data->death_lock);
			return ;
		}
	}
}
