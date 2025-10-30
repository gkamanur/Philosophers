/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 13:33:44 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/30 13:20:11 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	handle_single_philo(t_philo *philo)
{
	print_message(philo, "has taken a fork");
	while (!is_dead(philo->data))
		ft_usleep(1);
}

void	start_eating(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	first_fork = get_first_fork(philo);
	second_fork = get_second_fork(philo);
	take_forks(philo, first_fork, second_fork);
	eat_meal(philo);
	release_forks(first_fork, second_fork);
}

static void	start_sleeping_and_thinking(t_philo *philo)
{
	long	think_time;

	print_message(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	print_message(philo, "is thinking");
	if (philo->data->nb_philo % 2 != 0 && philo->data->nb_philo > 1)
	{
		think_time = (philo->data->time_to_eat * 2)
			- philo->data->time_to_sleep;
		if (think_time > 0 && think_time < 600
			&& philo->data->time_to_die > philo->data->time_to_eat * 2 + 5)
		{
			if (philo->data->time_to_die <= philo->data->time_to_eat
				+ philo->data->time_to_sleep + 10)
				ft_usleep(1);
			else if (think_time > 10)
				ft_usleep(think_time / 2);
			else
				ft_usleep(think_time);
		}
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
	apply_stagger_if_needed(philo);
	philosopher_cycle(philo);
	return (NULL);
}
