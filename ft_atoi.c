/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:07:56 by gkamanur          #+#    #+#             */
/*   Updated: 2025/10/27 09:20:10 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_error(void)
{
	write(2, "Error\n", 6);
	exit(EXIT_FAILURE);
}

static const char	*skip_spaces_and_sign(const char *str)
{
	while (*str && ((*str >= 9 && *str <= 13) || *str == ' '))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			ft_error();
		str++;
	}
	return (str);
}

static long	parse_digits(const char *str)
{
	long	result;
	int		has_digit;

	result = 0;
	has_digit = 0;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			has_digit = 1;
			result = result * 10 + (*str - '0');
			if ((result) > 2147483647)
				ft_error();
		}
		else
			ft_error();
		str++;
	}
	if (!has_digit)
		ft_error();
	return (result);
}

int	ft_atoi(const char *str)
{
	long	result;

	str = skip_spaces_and_sign(str);
	result = parse_digits(str);
	return ((int)result);
}
