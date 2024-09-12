/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:58:19 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/09/12 19:39:24 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	death_check(t_phil *phil)
{
	uint64_t	time_us;

	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	if ((time_us - phil->last_meal_time_us) > phil->phils_init->time_to_die)
	{
		printf("%s%li %i died%s\n", BLACK, time_us / 1000, phil->phil_id, NC);
		phil->is_alive = false;
		return (true);
	}
	return (false);
}

void	*monitor_routine(void *arg)
{
	t_phil	*phil_arr;
	int		num_of_phils;
	int		i;

	// usleep(10);
	phil_arr = (t_phil*) arg;
	num_of_phils = phil_arr->phils_init->num_of_phils;
	while (1)
	{
		i = 0;
		while (i < num_of_phils)
		{
			if (!phil_arr[i].is_alive)
			{
				// pthread_mutex_lock(&phil_arr->phils_init->simulation_mutex);
				// phil_arr->phils_init->simulation_run = false;
				// pthread_mutex_unlock(&phil_arr->phils_init->simulation_mutex);

				printf("SIMULATION STOPS...\n");
				// pthread_detach(phil_arr[i].thread_id);
				// cleanup(phil_arr->phils_init, phil_arr);
				return (NULL);
			}
			i++;
		}
	}
}


