/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:13:03 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/12/03 23:03:50 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// check correct input
// check that num of phils is in range 1 <= n <= 200
t_phils_init	*init_phils(t_phils_init *phils_init, char *argv[])
{
	int	i;

	phils_init->num_of_phils = ft_atoi(argv[1]);
	phils_init->time_to_die = (uint64_t) ft_atoi(argv[2]) * 1000;
	phils_init->time_to_eat = (uint64_t) ft_atoi(argv[3]) * 1000;
	phils_init->time_to_sleep = (uint64_t) ft_atoi(argv[4]) * 1000;
	phils_init->num_eat_times = -1;
	if (argv[5])
	{
		if (ft_atoi(argv[5]) == 0)
			return (NULL);
		phils_init->num_eat_times = ft_atoi(argv[5]);
	}
	phils_init->basetime_us = set_basetime_us();
	phils_init->stop_simulation = false;
	pthread_mutex_init(&phils_init->eat_times_mutex, NULL);
	pthread_mutex_init(&phils_init->stop_simulation_mutex, NULL);
	pthread_mutex_init(&phils_init->death_mutex, NULL);
	pthread_mutex_init(&phils_init->print_mutex, NULL);
	phils_init->phil_died = false;
	phils_init->forks = malloc(sizeof(pthread_mutex_t) * phils_init->num_of_phils);
	if (!phils_init->forks)
		return (NULL);
	i = 0;
	while (i < phils_init->num_of_phils)
	{
		pthread_mutex_init(&phils_init->forks[i], NULL);
		i++;
	}
	return (phils_init);
}
