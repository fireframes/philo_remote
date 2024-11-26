/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:58:19 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/11/26 18:35:16 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	death_check(t_phil *phil)
{
	uint64_t	time_us;

	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	if ((time_us - phil->last_meal_time_us) > phil->phils_init->time_to_die)
	{
		// pthread_mutex_lock(&phil->is_dead_mutex);
		// phil->is_dead = true;
		pthread_mutex_lock(&phil->phils_init->stop_simulation_mutex);
		phil->phils_init->stop_simulation = true;
		pthread_mutex_unlock(&phil->phils_init->stop_simulation_mutex);
		// pthread_mutex_unlock(&phil->is_dead_mutex);
		return (true);
	}
	return (false);
}

void	*monitor_routine(void *arg)
{
	uint64_t	time_us;
	t_phil		*phil_arr;
	int			num_of_phils;
	int			i;

	phil_arr = (t_phil*) arg;
	num_of_phils = phil_arr->phils_init->num_of_phils;
	while (1)
	{
		i = 0;
		while (i < num_of_phils)
		{
			if (death_check(&phil_arr[i]))
			{
				time_us = get_timestamp_us(phil_arr[i].phils_init->basetime_us);
				printf("%s%lu %d died%s\n", BLACK, time_us / 1000, 
					   phil_arr[i].phil_id, NC);
				return (NULL);
			}			
			i++;
			// usleep(100);
		}
	}
	return (NULL);
}


