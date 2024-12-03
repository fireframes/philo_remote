/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:17:04 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/12/03 21:47:53 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	set_basetime_us(void)
{
	uint64_t		basetime_us;
	struct timeval	tv;

	gettimeofday(&tv, NULL); // error check?
	basetime_us = tv.tv_sec * 1000000 + tv.tv_usec;
	return (basetime_us);
}

uint64_t	get_timestamp_us(uint64_t basetime_us)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL); // error check?
	return ((tv.tv_sec * 1000000 + tv.tv_usec) - basetime_us);
}


int	ft_atoi(const char *nptr)
{
	long int		num;
	int				neg;
	unsigned int	i;

	num = 0;
	neg = 1;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			neg = -1;
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		num = (num + (nptr[i] - 48)) * 10;
		i++;
	}
	num /= 10;
	return ((int) num * neg);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;
	int		diff;

	i = 0;
	diff = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	diff = (unsigned char) s1[i] - (unsigned char) s2[i];
	return (diff);
}
