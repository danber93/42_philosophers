#include "philosophers.h"

void	init_fork(t_fork *fork)
{
	fork->owner = -1;
	fork->used = 0;
	pthread_mutex_init(&(fork->f_access), NULL);
}

void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_phils)
	{
		table->phils[i].id = i;
		table->phils[i].n_eat = 0;
		table->phils[i].eat_start = 0;
		pthread_mutex_init(&(table->phils[i].p_access), NULL);
		table->phils[i].p_state = SLEEPING;
		init_fork(&table->phils[i].p_fork);
		table->phils[i].table = table;
		i++;
	}
}

void	init_mutex(t_table *table)
{
	pthread_mutex_init(&table->print_access, NULL);
	pthread_mutex_init(&table->start_access, NULL);
	pthread_mutex_init(&table->start_access, NULL);
}

void	init_all(t_table *table)
{
	init_mutex(table);
	init_philos(table);
	pthread_mutex_unlock(&(table->start_access));
}