/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:17:04 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/09/12 17:14:09 by mmaksimo         ###   ########.fr       */
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
