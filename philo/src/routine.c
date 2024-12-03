/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:08:50 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/12/03 23:24:51 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_phil *phil, char *state, char *color)
{
	uint64_t	time_us;
	bool		is_dead;

	pthread_mutex_lock(&phil->phils_init->death_mutex);
	is_dead = phil->phils_init->phil_died;
	pthread_mutex_unlock(&phil->phils_init->death_mutex);
	if (is_dead == true && ft_strcmp(state, "died") != 0)
		return ;
	time_us = get_timestamp_us(phil->phils_init->basetime_us);
	printf("%s%li %i %s%s\n", color, time_us / 1000, phil->phil_id, state, NC);
}

bool	simulation_ended(t_phil *phil)
{
	bool	ended;

	pthread_mutex_lock(&phil->phils_init->stop_simulation_mutex);
	ended = phil->phils_init->stop_simulation;
	pthread_mutex_unlock(&phil->phils_init->stop_simulation_mutex);
	return (ended);
}

void	thinking_subroutine(t_phil *phil)
{
	pthread_mutex_lock(&phil->phils_init->print_mutex);
	print_state(phil, "is thinking", PURPLE);
	pthread_mutex_unlock(&phil->phils_init->print_mutex);

}

void	eating_subroutine(t_phil *phil)
{
	phil->last_meal_time_us = get_timestamp_us(phil->phils_init->basetime_us);
	pthread_mutex_lock(&phil->phils_init->print_mutex);
	print_state(phil, "is eating", YELLOW);
	pthread_mutex_unlock(&phil->phils_init->print_mutex);

	usleep(phil->phils_init->time_to_eat);
	phil->times_eaten++;
}

void	sleeping_subroutine(t_phil *phil)
{
	pthread_mutex_lock(&phil->phils_init->print_mutex);
	print_state(phil, "is sleeping", GREEN);
	pthread_mutex_unlock(&phil->phils_init->print_mutex);

	usleep(phil->phils_init->time_to_sleep);
}

void	one_phil_fork_lock_unlock(t_phil *phil, char *what_to_do)
{
	if (ft_strcmp(what_to_do, "lock") == 0)
	{
		pthread_mutex_lock(&phil->phils_init->forks[0]);
		print_state(phil, "has taken a fork", CYAN);

	}
	else if (ft_strcmp(what_to_do, "unlock") == 0)
		pthread_mutex_unlock(&phil->phils_init->forks[0]);
}

void	unlock_forks_subroutine(t_phil *phil)
{
	int left_fork = phil->phil_id - 1;
	int right_fork = (phil->phil_id) % phil->phils_init->num_of_phils;

	pthread_mutex_unlock(&phil->phils_init->forks[left_fork]);
	pthread_mutex_unlock(&phil->phils_init->forks[right_fork]);
}

void single_phil_routine(t_phil *phil)
{
	one_phil_fork_lock_unlock(phil, "lock");
	if (simulation_ended(phil))
	{
		one_phil_fork_lock_unlock(phil, "unlock");
		return ;
	}
}

/*
The line if (left_fork < right_fork) implements a simple
but powerful deadlock prevention technique
called "resource hierarchy" or "total ordering of resources".
*/

void	lock_forks_subroutine(t_phil *phil)
{
	int left_fork = phil->phil_id - 1;
	int right_fork = (phil->phil_id) % phil->phils_init->num_of_phils;

	if (left_fork < right_fork)
	{
	pthread_mutex_lock(&phil->phils_init->forks[left_fork]);
	pthread_mutex_lock(&phil->phils_init->forks[right_fork]);
	}
	else
	{
		pthread_mutex_lock(&phil->phils_init->forks[right_fork]);
		pthread_mutex_lock(&phil->phils_init->forks[left_fork]);
	}

	// pthread_mutex_lock(&phil->phils_init->print_mutex);
	print_state(phil, "has taken a fork", CYAN);
	// pthread_mutex_unlock(&phil->phils_init->print_mutex);

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
		usleep(phil->phils_init->time_to_eat / 2);

	while (!simulation_ended(phil))
	{

		lock_forks_subroutine(phil);
		// if (simulation_ended(phil))
		// 	break ;
		eating_subroutine(phil);
		unlock_forks_subroutine(phil);
		if (simulation_ended(phil))
			break ;
		sleeping_subroutine(phil);
		thinking_subroutine(phil);

	}
	return (NULL);
}
