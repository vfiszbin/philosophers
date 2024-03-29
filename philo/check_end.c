/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_end.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 10:06:54 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/22 13:54:08 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(t_params *params, t_philo *philos, int i)
{
	pthread_mutex_lock(philos[i].check_death);
	if (get_time() - philos[i].time_last_meal > params->time_to_die)
	{
		pthread_mutex_lock(&params->end_check);
		params->end = 1;
		pthread_mutex_unlock(&params->end_check);
		display_state(&philos[i], "died", 1);
	}
	pthread_mutex_unlock(philos[i].check_death);
}

void	check_must_eat(t_params *params, int must_eat_satisfied)
{
	if (params->end == 0 && must_eat_satisfied == 1)
	{
		pthread_mutex_lock(&params->end_check);
		params->end = 1;
		pthread_mutex_unlock(&params->end_check);
		pthread_mutex_lock(&params->display);
		printf("Every philosopher have eaten at least %d meals !\n",
			params->nb_must_eat);
		pthread_mutex_unlock(&params->display);
	}
}

void	check_end(t_params *params, t_philo *philos)
{
	int	i;
	int	must_eat_satisfied;

	while (params->end == 0)
	{
		i = 0;
		must_eat_satisfied = 1;
		while (i < params->nb_philo)
		{
			pthread_mutex_lock(&params->check_meals);
			if (philos[i].nb_meals < params->nb_must_eat
				|| params->nb_must_eat == -1)
				must_eat_satisfied = 0;
			pthread_mutex_unlock(&params->check_meals);
			check_death(params, philos, i);
			if (params->end == 1)
				break ;
			i++;
			usleep(100);
		}
		check_must_eat(params, must_eat_satisfied);
		if (params->end == 1)
			break ;
	}
}
