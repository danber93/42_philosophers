#include "philosophers.h"

void	*ft_phil_live(void *arg)
{
	t_phil	*phil;
	t_phil	*next_phil;

	phil = (t_phil*)arg;
	next_phil = &(phil->table->phils[phil->id + 1 % phil->table->n_phils]);
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
	t_table *table;
	int		parse_input;

	if (!ft_parsing(table, argc, argv))
		return (0);
	table->phils = malloc(sizeof(t_phil) * table->n_phils);
	if (!table->phils)
		return (ft_error("Error while allocating phils", 0));
	table->state = RUNNING;
	init_all(table);
	return (0);
}