/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:09:02 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/11/26 20:15:41 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define NC		"\e[0m"
# define BLACK	"\e[1;30m"
# define BLUE	"\e[1;31m"
# define GREEN	"\e[1;32m"
# define YELLOW	"\e[1;33m"
# define PURPLE	"\e[1;35m"
# define CYAN	"\e[1;36m"

typedef struct s_phils_init
{
	int				num_of_phils;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				num_eat_times;
	uint64_t		basetime_us;
	bool			stop_simulation;
	pthread_mutex_t	eat_times_mutex;
	pthread_mutex_t	stop_simulation_mutex;
	pthread_mutex_t	*forks;
}	t_phils_init;

typedef struct s_phil
{
	int				phil_id;
	pthread_t		thread_id;
	uint64_t		basetime_phil_us;
	uint64_t		last_meal_time_us;
	pthread_mutex_t meal_time_mutex;
	int				times_eaten;
	// pthread_mutex_t	is_dead_mutex;
	// bool			is_dead;
	t_phils_init	*phils_init;

}	t_phil;

t_phils_init	*init_phils(t_phils_init *phils_init, int argc, char *argv[]);
void			*phil_routine(void *arg);
void			*monitor_routine(void *arg);
uint64_t		set_basetime_us(void);
uint64_t		get_timestamp_us(uint64_t basetime_us);
void			cleanup(t_phils_init *phils_init, t_phil *phil_arr);
int				ft_atoi(const char *nptr);

#endif
