/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:08:50 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/11/26 14:44:29 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	death_check(t_phil *phil)
{
	uint64_t	time_us;

	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	if ((time_us - phil->last_meal_time_us) > phil->phils_init->time_to_die)
	{
		pthread_mutex_lock(&phil->is_dead_mutex);
		phil->is_dead = true;
		pthread_mutex_unlock(&phil->is_dead_mutex);
		return (true);
	}
	return (false);
}

bool	simulation_ended(t_phil *phil)
{
	pthread_mutex_lock(&phil->phils_init->stop_simulation_mutex);
	if (phil->phils_init->stop_simulation)
	{
		pthread_mutex_unlock(&phil->phils_init->stop_simulation_mutex);
		return (true) ;
	}
	pthread_mutex_unlock(&phil->phils_init->stop_simulation_mutex);
	return (false);
}

void	thinking_subroutine(t_phil *phil)
{
	uint64_t	time_us;

	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	printf("%s%li %i is thinking%s\n",
			PURPLE, (time_us / 1000),
			phil->phil_id, NC);
}

void	eating_subroutine(t_phil *phil)
{
	uint64_t	time_us;

	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	phil->last_meal_time_us = get_timestamp_us(phil->phils_init->basetime_us);
	printf("%s%li %i is eating%s\n", YELLOW, time_us / 1000, phil->phil_id, NC);
	usleep(phil->phils_init->time_to_eat);

}

void	sleeping_subroutine(t_phil *phil)
{
	uint64_t	time_us;

	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	printf("%s%li %i is sleeping%s\n", BLUE, (time_us / 1000), phil->phil_id, NC);
	usleep(phil->phils_init->time_to_sleep);
}

void	lock_fork_subroutine(t_phil *phil, int fork_num)
{
	uint64_t	time_us;

	pthread_mutex_lock(&phil->phils_init->forks[phil->phil_id - fork_num]);
	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	printf("%s%li %i has taken the fork %s\n", CYAN,
			time_us / 1000, phil->phil_id, NC);
}

void	unlock_fork_subroutine(t_phil *phil)
{
	pthread_mutex_unlock(&phil->phils_init->forks[phil->phil_id - 1]);
	if (phil->phils_init->num_of_phils > 1)
		pthread_mutex_unlock(&phil->phils_init->forks[phil->phil_id - 2]);
}
void single_phil_routine(t_phil *phil)
{
	return ;
}


void	*phil_routine(void *arg)
{
	t_phil		*phil;

	phil = (t_phil*) arg;
	if (phil->phils_init->num_of_phils == 1)
	{
		single_phil_routine(phil);
		return (NULL);
	}
	if (phil->phil_id % 2 == 0)
		usleep(phil->phils_init->time_to_eat);
	while (1)
	{
		if (simulation_ended(phil))
			break ;
		lock_fork_subroutine(phil, 1);
		eating_subroutine(phil);

		unlock_fork_subroutine(phil);
		sleeping_subroutine(phil);

		thinking_subroutine(phil);
	}
	return (NULL);
}

