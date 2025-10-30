/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:50:13 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/30 11:03:34 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_data *data, int argc, char **argv)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->nb_meals = -1;
	if (argc == 6)
		data->nb_meals = ft_atoi(argv[5]);
	else
		data->nb_meals = -1;
	data->dead = 0;
	data->finished = 0;
	data->start_time = get_current_time();
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->write_lock, NULL) != 0
		|| pthread_mutex_init(&data->death_lock, NULL) != 0
		|| pthread_mutex_init(&data->meal_lock, NULL) != 0)
	{
		printf("Error: Mutex initialization failed\n");
		return (1);
	}
	return (0);
}

int	alloc_resources(t_data *data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->forks || !data->philos)
	{
		printf("Error: Memory allocation failed\n");
		return (1);
	}
	return (0);
}

void	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}
