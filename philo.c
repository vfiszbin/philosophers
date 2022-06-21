/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo->c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student->42->fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 16:30:44 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/19 17:41:38 by vfiszbin         ###   ########->fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_params(int argc, char **argv, t_params *params)
{
	params->end = 0;
	params->nb_philo = ft_atoi(argv[1]);
	params->time_to_die = ft_atoi(argv[2]);
	params->time_to_eat = ft_atoi(argv[3]);
	params->time_to_sleep = ft_atoi(argv[4]);
	if (params->nb_philo < 1 || params->time_to_die < 0
		|| params->time_to_eat < 0 || params->time_to_sleep < 0)
	{
		printf("Incorrect arguments\n");
		return (1);
	}
	if (argc == 6)
	{
		params->nb_must_eat = ft_atoi(argv[5]);
		if (params->nb_must_eat < 0)
		{
			printf("Incorrect arguments\n");
			return (1);
		}
	}
	else
		params->nb_must_eat = -1;
	return (0);
}

void free_philos(t_params *params, t_philo *philos)
{
	int i;

	i = 0;
	while (i < params->nb_philo)
	{
		pthread_mutex_destroy(philos[i].right_fork);
		free(philos[i].right_fork);
		pthread_mutex_destroy(philos[i].check_death);
		free(philos[i].check_death);
		i++;
	}
	pthread_mutex_destroy(&params->display);
	free(philos);
}

void wait_for_threads(t_params *params, t_philo *philos)
{
	int i;

	i = 0;
	while (i < params->nb_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

void check_end(t_params *params, t_philo *philos)
{
	int i;
	int must_eat_satisfied;


	while (params->end == 0)
	{
		i = 0;
		must_eat_satisfied = 1;
		while (i < params->nb_philo)
		{
			if (philos[i].nb_meals < params->nb_must_eat || params->nb_must_eat == -1)
				must_eat_satisfied = 0;

			pthread_mutex_lock(philos[i].check_death);
			if (get_time() - philos[i].time_last_meal > params->time_to_die)
			{
				display_state(&philos[i], "died");
				params->end = 1;
			}
			pthread_mutex_unlock(philos[i].check_death);
			if (params->end == 1)
				break ;
			i++;
		}
		if (params->end == 0 && must_eat_satisfied == 1)
		{
			params->end = 1;
			pthread_mutex_lock(&params->display);
			printf("Every philosopher have eaten at least %d meals !\n", params->nb_must_eat);
			pthread_mutex_unlock(&params->display);
			// i = 0;
			// while (i < params->nb_philo)
			// {
			// 	printf("%d : %d\n", i, philos[i].nb_meals);
			// 	i++;
			// }
			break;
		}
		usleep(50);
	}
}


void display_state(t_philo *philo, char *msg)
{
	struct timeval time;

	pthread_mutex_lock(philo->display);
	if (philo->params->end == 0)
	{
		gettimeofday(&time, NULL);
		printf("%ld %d %s\n", ((time.tv_sec * 1000) + (time.tv_usec / 1000)) - philo->start_time, philo->num, msg);
	}
	pthread_mutex_unlock(philo->display);
}

void *routine(void *philo_arg)
{
	t_philo *philo;

	philo = (t_philo *)philo_arg;
	while (philo->params->end == 0)
	{
		eat_philo(philo);
		if (philo->params->end != 0)
			break;
		sleep_philo(philo);
		if (philo->params->end != 0)
			break;
		think_philo(philo);
	}
	return (NULL);
}

int start_threads(t_params *params, t_philo *philos)
{
	int i;

	i = 0;
	while (i < params->nb_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &(philos[i])) != 0)
			return (1); //handle error
		i++;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_params params;
	t_philo *philos;

	if (argc != 5 && argc != 6)
	{
		printf("Wrong number of arguments\n");
		return (1);
	}
	if (get_params(argc, argv, &params) == 1)
		return (1);
	philos = create_philos(&params);
	if (!philos)
		return (1);
	start_threads(&params, philos);
	check_end(&params, philos);
	wait_for_threads(&params, philos);
	free_philos(&params, philos);
}