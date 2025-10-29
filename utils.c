/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:50:54 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/29 13:15:54 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		write (2, "gettimeofday fucked up!!!\n", 26);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long time)
{
	long	start;
	long	elapsed;

	start = get_current_time();
	while (1)
	{
		elapsed = get_current_time() - start;
		if (elapsed >= time)
			break ;
		if (time - elapsed > 1)
			usleep((time - elapsed) * 1000 / 2);
		else
			usleep(100);
	}
}

void	print_message(t_philo *philo, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->write_lock);
	if (!is_dead(philo->data) && !is_finished(philo->data))
	{
		timestamp = get_current_time() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, message);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
}
