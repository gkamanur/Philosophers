/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stagger.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 11:05:07 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/30 11:41:13 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	calculate_stagger(t_philo *philo)
{
	long	stagger;

	if (philo->data->nb_philo > 100)
		stagger = 50;
	else if (philo->data->nb_philo > 50)
		stagger = 30;
	else if (philo->data->nb_philo > 10)
		stagger = 15;
	else if (philo->data->nb_philo > 3)
		stagger = 1;
	else
		stagger = 0;
	if (philo->data->time_to_die <= philo->data->time_to_eat
		+ philo->data->time_to_sleep + 10)
		stagger = 0;
	else if (philo->data->time_to_die <= philo->data->time_to_eat
		+ philo->data->time_to_sleep + 50)
		stagger /= 3;
	return (stagger);
}

void	apply_stagger_if_needed(t_philo *philo)
{
	long	stagger;

	if (philo->id % 2 != 0)
		return ;
	stagger = calculate_stagger(philo);
	if (stagger > 0)
		ft_usleep(stagger);
}
