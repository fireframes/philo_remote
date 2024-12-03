/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:58:19 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/12/03 23:15:39 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	death_check(t_phil *phil)
{
	uint64_t	time_us;

	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	if ((time_us - phil->last_meal_time_us) > phil->phils_init->time_to_die)
	{
		pthread_mutex_lock(&phil->phils_init->death_mutex);
		phil->phils_init->phil_died = true;
		pthread_mutex_unlock(&phil->phils_init->death_mutex);

		pthread_mutex_lock(&phil->phils_init->stop_simulation_mutex);
		phil->phils_init->stop_simulation = true;
		pthread_mutex_unlock(&phil->phils_init->stop_simulation_mutex);

		pthread_mutex_lock(&phil->phils_init->print_mutex);
		print_state(phil, "died", BLACK);
		pthread_mutex_unlock(&phil->phils_init->print_mutex);

		return (true);
	}
	return (false);
}

bool	eaten_enough(t_phil *phil)
{
	if (phil->phils_init->num_eat_times == -1)
		return (false);
	pthread_mutex_lock(&phil->phils_init->eat_times_mutex);
	if (phil->times_eaten >= phil->phils_init->num_eat_times)
	{
		pthread_mutex_lock(&phil->phils_init->stop_simulation_mutex);
		phil->phils_init->stop_simulation = true;
		pthread_mutex_unlock(&phil->phils_init->stop_simulation_mutex);
		pthread_mutex_unlock(&phil->phils_init->eat_times_mutex);
		return (true);
	}
	pthread_mutex_unlock(&phil->phils_init->eat_times_mutex);
	return (false);
}

void	*monitor_routine(void *arg)
{
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
			if (death_check(&phil_arr[i]) || eaten_enough(&phil_arr[i]))
				return (NULL);
			i++;
			// usleep(100);
		}
	}
	return (NULL);
}
