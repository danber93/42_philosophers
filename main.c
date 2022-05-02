/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <dbertill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 15:02:17 by dbertill          #+#    #+#             */
/*   Updated: 2022/05/02 20:01:25 by dbertill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
* The function to be executed by the thread should take a
* void * parameter and return a void * result .
*/
void    *thread_function(void *arg, pthread_mutex_t m1)
{
	// Cast the parameter into whatever type is appropriate.
	int  *incoming = (int*)arg;
	
	// Do whatever is necessary using * incoming as the argument.
	printf("%i\n", *incoming);
	// The thread terminates when this function returns.
	return (NULL);
}

void	*eating(t_table *game, int phil)
{
	int	n_phils;
	// Fork at their right
	int	fork_1;
	// Fork at their left
	int	fork_2;

	n_phils = game->n_philosophers;
	if (phil == 0 || phil == n_phils)
	{
		fork_1 = game->forks[0];
		fork_2 = game->forks[n_phils - 1];
	}
	else
	{
		fork_1 = game->forks[n_phils];
		fork_2 = game->forks[n_phils + 1];
	}
}

t_table *init_game(int argc, char **argv)
{
	t_table *game;

	game = (t_table *)malloc(sizeof(t_table) * 1);
	if (!game)
		return (0);
	
	game->n_philosophers = argv[1];
	game->time_to_die = argv[2];
	game->time_to_eat = argv[3];
	game->time_to_sleep = argv[4];
	game->forks = malloc(sizeof(int), game->n_philosophers);
	if (argc == 6)
		game->n_meals = argv[5];
	return (game);
}

void	*ft_calloc(size_t size, size_t count)
{
	size_t	i;
	void *ptr;

	ptr = malloc(size * count);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < count)
	{
		*(char *)(ptr +i) = 0;
		i++;
	}
	return (ptr);
}

t_philosopher **init_phils(t_table *game)
{
	int	i;
	t_philosopher **phils;

	phils = ft_calloc(sizeof(t_philosopher *), game->n_philosophers);
	if (!phils)
		return (NULL);
	i = 0;
	while (i < game->n_philosophers)
	{
		phils[i] = malloc(sizeof(t_philosopher));
		if (!(phils[i]))
			return (NULL);
		phils[i]->is_eating = 0;
		phils[i]->is_sleeping = 0;
		phils[i]->is_thinking = 0;
		phils[i]->countdown_starving = game->time_to_die;
	}
}

/* • Your(s) program(s) should take the following arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]*/
/* 
- number_of_philosophers: The number of philosophers and also the number
of forks.
- time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
- time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
- time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
- number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.
*/
int main(int argc, char **argv)
{
	pthread_t	thread_id;
	void		*thread_result;
	int			value;
	int			n_phil;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			**n_times_eat;
    t_table *game;
	t_philosopher **phils;
	
	if (argc > 4 && argc < 7)
	{
		printf("Error. Wrong numbers of arguments.\n");
		printf("The program accepts exact these arguments:\n");
		printf("-- number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	game = init_game(argc, argv);
	phils = init_phils(game);

	// Put something meaningful into value
	value = 42;

	// Create the thread, passing &value for the argument
	pthread_create(&thread_id, NULL, thread_function, &value);

	// The main program continues while the thread executes.

	// Wait for the thread to terminate.
	pthread_join(thread_id, &thread_result);

	printf("The other thread finished.\n");
	// Only the main thread is running now.
	return (0);
}
