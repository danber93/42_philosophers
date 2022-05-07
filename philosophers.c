/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <dbertill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/07 16:35:51 by dbertill          #+#    #+#             */
/*   Updated: 2022/05/07 16:35:51 by dbertill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_live(void *arg)
{
	t_phil	*phil;
	t_phil	*next_phil;

	phil = (t_phil *)arg;
	if (phil->id == phil->table->n_phils - 1)
		next_phil = &(phil->table->phils[0]);
	else
		next_phil = &(phil->table->phils[phil->id + 1]);
	while (phil->table->state == RUNNING)
	{
		if (phil->p_state == SLEEPING)
			ft_sleep(phil);
		if (phil->p_state == THINKING)
			ft_think(phil, next_phil);
		if (phil->p_state == EATING)
			ft_eat(phil, next_phil);
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_table	*table;
	int		parse_input;
	int		i;

	table = malloc(sizeof(t_table));
	if (!table)
		return (ft_error("Table malloc failed.", 0));
	if (!ft_parsing(table, argc, argv))
		return (0);
	table->phils = malloc(sizeof(t_phil) * table->n_phils);
	if (!table->phils)
		return (ft_error("Error while allocating phils", 0));
	table->state = RUNNING;
	ft_init_all(table);
	while (!ft_anyone_dead(table) && !ft_full_belly(table))
		usleep(CHECK_EXIT_CYCLE_MS);
	i = 0;
	while (i < table->n_phils)
		pthread_join(table->phils[i++].p_thread, NULL);
	free(table->phils);
	return (0);
}
