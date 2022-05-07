/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <dbertill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 16:33:24 by dbertill          #+#    #+#             */
/*   Updated: 2022/05/07 16:50:42 by dbertill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_init_fork(t_fork *fork)
{
	fork->owner = -1;
	fork->used = 0;
	pthread_mutex_init(&(fork->f_access), NULL);
}

void	ft_init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_phils)
	{
		table->phils[i].id = i;
		table->phils[i].n_eat = 0;
		table->phils[i].eat_start = 0;
		pthread_mutex_init(&(table->phils[i].access), NULL);
		table->phils[i].p_state = SLEEPING;
		ft_init_fork(&table->phils[i].fork);
		table->phils[i].table = table;
		pthread_create(&table->phils[i].thread, NULL, ft_live,
			(void *)(table->phils + i));
		i++;
	}
}

void	ft_init_mutex(t_table *table)
{
	pthread_mutex_init(&table->print_access, NULL);
	pthread_mutex_init(&table->start_access, NULL);
	pthread_mutex_lock(&table->start_access);
}

void	ft_init_all(t_table *table)
{
	ft_init_mutex(table);
	ft_init_philos(table);
	pthread_mutex_unlock(&table->start_access);
}
