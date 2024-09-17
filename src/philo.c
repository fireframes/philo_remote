/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 15:32:03 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/09/17 23:10:02 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_argc(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("program takes 4[5] arguments: number_of_philosophers time_to_die time_to_eat time_to_sleep [num_t_phil_must_eat]\n");
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
		pthread_mutex_destroy(&phils_init->forks[j]); // error check
		j++;
	}
	free(phils_init->forks); // frees all?
	free(phil_arr); // frees all?
}

// void	init_monitor(t_phil *phil_arr)
// {

// 	pthread_t	monitor_th;

// 	if (pthread_create(&monitor_th, NULL, monitor_routine, (void*) phil_arr) != 0)
// 	{
// 		write(STDERR_FILENO, "Failed to create thread\n", 25);
// 		return (4);
// 	}
// 	if (pthread_join(monitor_th, NULL) != 0)
// 	{
// 		write(STDERR_FILENO, "Failed to join thread\n", 23);
// 		return (5);
// 	}
// }

int	main(int argc, char *argv[])
{
	t_phils_init	phils_init;
	t_phil			*phil_arr;

	if (!check_argc(argc))
		return (1);

	init_phils(&phils_init, argc, argv);
	phil_arr = malloc(sizeof(t_phil) * phils_init.num_of_phils);
	if (phil_arr == NULL)
	{
		write(STDERR_FILENO, "Failed to allocate array of phils\n", 35);
		return (2);
	}
	int i = 0;
	while (i < phils_init.num_of_phils)
	{
		phil_arr[i].phil_id = i + 1;
		phil_arr[i].last_meal_time_us = 0;
		phil_arr[i].is_dead = false;
		phil_arr[i].phils_init = &phils_init;
		if (pthread_create(&phil_arr[i].thread_id, NULL, phil_routine, (void*) &phil_arr[i]) != 0) //, NULL) != 0) //
		{
			write(STDERR_FILENO, "Failed to create thread\n", 25);
			return (3);
		}
		i++;
	}
	pthread_t	monitor_th;

	if (pthread_create(&monitor_th, NULL, monitor_routine, (void*) phil_arr) != 0)
	{
		write(STDERR_FILENO, "Failed to create thread\n", 25);
		return (4);
	}
	if (pthread_join(monitor_th, NULL) != 0)
	{
		write(STDERR_FILENO, "Failed to join thread\n", 23);
		return (5);
	}
	cleanup(&phils_init, phil_arr);


	printf("SIMULATION ENDED.\n");
	return (0);
}
