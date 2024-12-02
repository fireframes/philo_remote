/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 15:32:03 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/12/03 22:25:55 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_argc(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("program takes 4[5] arguments: \
		number_of_philosophers time_to_die time_to_eat \
		time_to_sleep [num_t_phil_must_eat]\n");
		return (0);
	}
	return (1);
}

void	cleanup(t_phils_init *phils_init, t_phil *phil_arr)
{
	int j = 0;

	while (j < phils_init->num_of_phils)
	{
		if (pthread_join(phil_arr[j].thread_id, NULL) != 0)
		{
			write(STDERR_FILENO, "Failed to join thread\n", 23);
			break ;
		}
		j++;
	}
	j = 0;
	while (j < phils_init->num_of_phils)
	{
		pthread_mutex_destroy(&phils_init->forks[j]);
		j++;
	}
	free(phils_init->forks);
	free(phil_arr);
}

int	main(int argc, char *argv[])
{
	t_phils_init	phils_init;
	t_phil			*phil_arr;

	if (!check_argc(argc))
		return (1);
	if (!init_phils(&phils_init, argv))
	{
		write(STDERR_FILENO, "Failed to initialize simulation\n", 33);
		return (2);
	}
	phil_arr = malloc(sizeof(t_phil) * phils_init.num_of_phils);
	if (phil_arr == NULL)
	{
		write(STDERR_FILENO, "Allocation failed\n", 19);
		return (2);
	}
	int i = 0;
	while (i < phils_init.num_of_phils)
	{
		phil_arr[i].phil_id = i + 1;
		phil_arr[i].last_meal_time_us = 0;
		phil_arr[i].phils_init = &phils_init;
		phil_arr[i].times_eaten = 0;
		if (pthread_create(&phil_arr[i].thread_id, NULL, phil_routine, (void*) &phil_arr[i]) != 0)
		{
			write(STDERR_FILENO, "Failed to create thread\n", 25);
			free(phils_init.forks);
			free(phil_arr);
			return (3);
		}
		i++;
	}

	pthread_t	monitor_th;

	if (pthread_create(&monitor_th, NULL, monitor_routine, (void*) phil_arr) != 0)
	{
		write(STDERR_FILENO, "Failed to create thread\n", 25);
		free(phils_init.forks);
		free(phil_arr);
		return (3);
	}

	int j = 0;
	while (j < phils_init.num_of_phils)
	{
		if (pthread_join(phil_arr[j].thread_id, NULL))
		{
			write(STDERR_FILENO, "Failed to join thread\n", 23);
			return (4);
		}
		j++;
	}
	if (pthread_join(monitor_th, NULL) != 0)
	{
		write(STDERR_FILENO, "Failed to join thread\n", 23);
		return (4);
	}

	i = 0;
	while (i < phils_init.num_of_phils)
	{
		pthread_mutex_destroy(&phils_init.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&phils_init.eat_times_mutex);
	pthread_mutex_destroy(&phils_init.stop_simulation_mutex);
	pthread_mutex_destroy(&phils_init.death_mutex);
	pthread_mutex_destroy(&phils_init.print_mutex);

	free(phils_init.forks);
	free(phil_arr);

	return (0);
}
