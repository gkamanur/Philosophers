/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:49:54 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/29 12:02:50 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	if (parse_args(data, argc, argv))
		return (1);
	if (alloc_resources(data))
		return (1);
	if (init_mutexes(data))
		return (1);
	init_forks(data);
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_count = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philo];
		data->philos[i].data = data;
		i++;
	}
	return (0);
}

int	start_simulation(t_data *data)
{
	int	i;

	data->start_time = get_current_time();
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Thread creation failed\n");
			return (1);
		}
		i++;
	}
	monitor_death(data);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep \
			[number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (1);
	}
	if (init_data(&data, argc, argv) != 0)
		return (1);
	if (init_philos(&data) != 0)
	{
		cleanup(&data);
		return (1);
	}
	if (start_simulation(&data) != 0)
	{
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}
