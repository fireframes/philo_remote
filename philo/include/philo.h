/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:09:02 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/12/03 23:03:50 by mmaksimo         ###   ########.fr       */
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
	pthread_mutex_t	*forks;
	pthread_mutex_t	eat_times_mutex;
	pthread_mutex_t	stop_simulation_mutex;
	bool			stop_simulation;
	pthread_mutex_t	death_mutex;
	bool			phil_died;
	pthread_mutex_t	print_mutex;

}	t_phils_init;

typedef struct s_phil
{
	int				phil_id;
	pthread_t		thread_id;
	uint64_t		basetime_phil_us;
	uint64_t		last_meal_time_us;
	int				times_eaten;
	t_phils_init	*phils_init;
}	t_phil;

t_phils_init	*init_phils(t_phils_init *phils_init, char *argv[]);
void			*phil_routine(void *arg);
void			*monitor_routine(void *arg);
uint64_t		set_basetime_us(void);
uint64_t		get_timestamp_us(uint64_t basetime_us);
void			print_state(t_phil *phil, char *state, char *color);
bool			simulation_ended(t_phil *phil);
int				ft_atoi(const char *nptr);
int				ft_strcmp(const char *s1, const char *s2);
void			cleanup(t_phils_init *phils_init, t_phil *phil_arr);

#endif
