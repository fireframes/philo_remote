/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:08:50 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/09/12 19:35:46 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	simulation_check(t_phil *phil)
{
	pthread_mutex_lock(&phil->phils_init->simulation_mutex);
	if (phil->phils_init->simulation_run == false)
	{
		pthread_mutex_unlock(&phil->phils_init->simulation_mutex);
		return (false);
	}
	pthread_mutex_unlock(&phil->phils_init->simulation_mutex);
	return (true);
}

void	*phil_routine(void *arg)
{
	uint64_t	time_us;
	t_phil		*phil;

	phil = (t_phil*) arg;

	if (phil->phils_init->num_of_phils > 1 && phil->phil_id % 2 == 0)
	{
		time_us = get_timestamp_us(phil->phils_init->basetime_us);
		printf("%s%li %i is thinking%s\n", PURPLE, (time_us / 1000), phil->phil_id, NC);
		usleep(10);
	}
	while (1) // var simulation_running is 1 -- noone died
	{
		pthread_mutex_lock(&phil->phils_init->forks[phil->phil_id - 1]);
		// phil->forks_taken = 1; // do I need this or make separate logic
		time_us = get_timestamp_us(phil->phils_init->basetime_us);
		printf("%s%li %i has taken the fork %s\n", CYAN, (time_us / 1000), phil->phil_id, NC);

		if (phil->phils_init->num_of_phils > 1)
		{
			pthread_mutex_lock(&phil->phils_init->forks[phil->phil_id - 2]);
			// phil->forks_taken = 2;
			time_us = get_timestamp_us(phil->phils_init->basetime_us);
			printf("%s%li %i has taken the fork %s\n", CYAN, (time_us / 1000), phil->phil_id, NC);
		}

		if (simulation_check(phil) == false)
			break ;
		//|| (phil->phils_init->num_of_phils == 1 && phil->forks_taken != 2))

		phil->last_meal_time_us = get_timestamp_us(phil->phils_init->basetime_us);
		printf("%s%li %i is eating%s\n", YELLOW, (time_us / 1000), phil->phil_id, NC);
		usleep(phil->phils_init->time_to_eat);

		// phil->forks_taken = 0;
		pthread_mutex_unlock(&phil->phils_init->forks[phil->phil_id - 1]);
		if (phil->phils_init->num_of_phils > 1)
				pthread_mutex_unlock(&phil->phils_init->forks[phil->phil_id - 2]);

		time_us = get_timestamp_us(phil->phils_init->basetime_us);
		printf("%s%li %i is sleeping%s\n", BLUE, (time_us / 1000), phil->phil_id, NC);
		usleep(phil->phils_init->time_to_sleep);

		if (simulation_check(phil) == false)
			break ;

		time_us = get_timestamp_us(phil->phils_init->basetime_us);
		printf("%s%li %i is thinking%s\n", PURPLE, (time_us / 1000), phil->phil_id, NC);

	}
	return (NULL);
}
