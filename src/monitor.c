/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:58:19 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/09/30 23:47:07 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


// bool	simulation_check(t_phil *phil)
// {
// 	pthread_mutex_lock(&phil->phils_init->simulation_mutex);
// 	if (phil->phils_init->simulation_run == false)
// 	{
// 		pthread_mutex_unlock(&phil->phils_init->simulation_mutex);
// 		return (false);
// 	}
// 	pthread_mutex_unlock(&phil->phils_init->simulation_mutex);
// 	return (true);
// }


void	*monitor_routine(void *arg)
{
	t_phil	*phil_arr;
	int		num_of_phils;
	int		i;

	phil_arr = (t_phil*) arg;
	num_of_phils = phil_arr->phils_init->num_of_phils;
	while (1)
	{
		i = 0;
		while (i < num_of_phils)
		{
			pthread_mutex_lock(&phil_arr[i].is_dead_mutex);
			if (phil_arr[i].is_dead)
			{
				pthread_mutex_lock(&phil_arr[i].phils_init->stop_simulation_mutex);
				phil_arr[i].phils_init->stop_simulation = true;
				pthread_mutex_unlock(&phil_arr[i].phils_init->stop_simulation_mutex);

				uint64_t	time_us;
				time_us = get_timestamp_us(phil_arr[i].phils_init->basetime_us);
				printf("%s%li %i died%s\n", BLACK, time_us / 1000, phil_arr[i].phil_id, NC);
				
				pthread_mutex_unlock(&phil_arr[i].is_dead_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&phil_arr[i].is_dead_mutex);
			i++;
			usleep(10);
		}
	}
	return (NULL);
}


