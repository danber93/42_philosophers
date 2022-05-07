#include "philosophers.h"

int	ft_check_input(t_table *table, int argc)
{
	if (table->n_phils < 1)
		return (ft_error("Wrong number of philosophers. Must be greater then 0.", 0));
	if (table->time_to_die < 1)
		return (ft_error("time_to_die too small. Must be greater than 0.", 0));
	if (table->time_to_eat < 1)
		return (ft_error("time_to_eat too small. Must be greater than 0.", 0));
	if (table->time_to_sleep < 1)
		return (ft_error("time_to_sleep too small. Must be greater than 0.", 0));
	if (argc == 6)
		if (table->n_meals < 1)
			return (ft_error("Number of times each philosophers has to eat too small. Must be greather than 0.", 0));
	return (1);
}

int	ft_parsing(t_table *table, int argc, char *argv[])
{
	if (!(argc == 5 || argc == 6))
		return (ft_error("Wrong number of args. It must be 4 or 5.", 0));
	table->n_phils = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->n_meals = -1;
	if (argc == 6)
		table->n_meals = ft_atoi(argv[5]);
	if (ft_check_input(table, argc) == 0)
		return (0);
	return (1);
}