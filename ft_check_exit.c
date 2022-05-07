/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <dbertill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 16:29:40 by dbertill          #+#    #+#             */
/*   Updated: 2022/05/07 16:29:40 by dbertill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_anyone_dead(t_table *table)
{
	int	i;
	int	runtime;

	i = 0;
	while (i < table->n_phils)
	{
		pthread_mutex_lock(&(table->phils[i].p_access));
		runtime = elapsed_time();
		if (runtime - table->phils[i].eat_start
			> table->time_to_die + DEATH_TOLL_MS)
		{
			ft_display(&table->phils[i], runtime, "died");
			table->state = FINISHED;
			pthread_mutex_unlock(&(table->phils[i].p_access));
			return (1);
		}
		pthread_mutex_unlock(&(table->phils[i].p_access));
		i++;
	}
	return (0);
}

int	ft_full_belly(t_table *table)
{
	int	i;

	if (table->n_meals == -1)
		return (0);
	i = 0;
	while (i < table->n_meals)
	{
		pthread_mutex_lock(&(table->phils[i].p_access));
		if (table->phils[i].n_eat < table->n_meals)
		{
			pthread_mutex_unlock(&(table->phils[i].p_access));
			return (0);
		}
		pthread_mutex_unlock(&(table->phils[i].p_access));
		i++;
	}
	table->state = FINISHED;
	return (1);
}
