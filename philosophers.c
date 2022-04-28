#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct	s_phil
{
	unsigned int	countdown_eating;
	unsigned int	countdown_diying;
	unsigned int	countdown_sleeping;
	int				is_eating;
	int				is_sleeping;
	int				is_thinking;
	unsigned int	last_check;
	pthread_t		thread_id;
	int				id;
	int				n_meals;
}				t_phil;

typedef struct	s_table
{
	unsigned int	time_to_die;
	unsigned int	time_to_sleep;
	unsigned int	time_to_eat;
	int				n_phils;
	int				n_meals;
	t_phil			*phils;
	pthread_mutex_t	*forks;
}				t_table;

unsigned int	ft_atoi(char *str)
{
	int				i;
	unsigned int	res;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		return (0);
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
		res = (res * 10) + (str[i++] - '0');
	if (str[i - 1] < '0' || str[i - 1] > '9')
		return (0);
	return (res);
}

int		ft_anyone_dead(int n_phils, t_phil *phils, unsigned int time_to_die)
{
	int	i;

	i = 0;
	while (i < n_phils)
	{
		if (phils[i].countdown_diying == 0 || phils[i].countdown_diying > time_to_die)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

unsigned int	ft_diff_ms(unsigned int ms, unsigned int last_check)
{
	if (ms < last_check)
		return (999999 - last_check + ms);
	return (ms - last_check);
}

void	update_ct(int	n_phils, t_phil *phils)
{
	struct timeval	current_time;
	int				i;

	i = 0;
	gettimeofday(&current_time, NULL);
	while (i < n_phils)
	{
		if ((phils[i]).is_eating)
		{
			phils[i].countdown_eating -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
		}
		if ((phils[i]).is_sleeping)
		{
			phils[i].countdown_sleeping -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
			phils[i].countdown_diying -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
			if (phils[i].countdown_sleeping <= 0)
				phils[i].countdown_sleeping = 0;
		}
		if ((phils[i]).is_thinking)
		{
			phils[i].countdown_diying -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
		}
		phils[i].last_check = current_time.tv_usec;
		printf("Phil %i has still %u milliseconds before dying.\n", i, phils[i].countdown_diying / 1000);
		i++;
	}
}

void	ft_init_phils_forks(t_table *table)
{
	int	i;
	struct timeval current_time;

	i = 0;
	while (i < table->n_phils)
	{
		gettimeofday(&current_time, NULL);
		table->phils[i].countdown_diying = table->time_to_die;
		table->phils[i].countdown_eating = 0;
		table->phils[i].countdown_sleeping = 0;
		table->phils[i].is_thinking = 1;
		table->phils[i].is_sleeping = 0;
		table->phils[i].is_eating = 0;
		table->phils[i].n_meals = table->n_meals;
		table->phils[i].id = i;
		table->phils[i].last_check = current_time.tv_usec;
		pthread_mutex_init(&(table->forks[i]), NULL);
		i++;
	}
}

t_table *ft_init_table(int	argc, char *argv[])
{
	t_table	*table;

	table = (t_table *)malloc(sizeof(t_table) * 1);
	if (!table)
		return (0);
	table->n_phils = ft_atoi(argv[1]);
	printf("n_phils = %i\n", table->n_phils);
	table->phils = (t_phil *)malloc(sizeof(t_phil) * (table->n_phils));
	if (!(table->phils))
		return (NULL);
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * argc);
	if (!(table->forks))
		return (NULL);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->n_meals = ft_atoi(argv[5]);
	ft_init_phils_forks(table);
	return (table);
}

int	ft_check_input(int argc, char *argv[])
{
	if (!(argc > 4 && argc < 7))
	{
		printf("Error. Wrong numbers of arguments.\n");
		printf("The program accepts exact these arguments:\n");
		printf("  1- number_of_philosophers\n  2- time_to_die (in milliseconds)\n");
		printf("  3- time_to_eat (in milliseconds)\n  4- time_to_sleep(in milliseconds)\n");
		printf("  5- [number_of_times_each_philosopher_must_eat] (optional)\n");
		return (0);
	}
	return (1);
}

void	ft_eat(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_phils)
	{
		if (table->phils[i].is_eating)
		{
			
		}
	}
}

int	main(int argc, char *argv[])
{
	int		dead;
	int		i;
	t_table	*table;

	if (!(ft_check_input(argc, argv)))
		return (0);
	table = ft_init_table(argc, argv);
	dead = 0;
	while (!dead)
	{
		// pthread_create(&(phils[i].thread_id), NULL, ft_try_eat, phils[i])
		update_ct(table->n_phils, (table->phils));
		dead = ft_anyone_dead(table->n_phils, table->phils, table->time_to_die);
		dead = 1;
	}
}
/* 
int main()
{
	int	n_phils;
	int	dead;
	int	i;
	t_phil *phils;
	pthread_mutex_t *forks;
	struct timeval current_time;
	unsigned int	time_to_die;
	t_table			*table;

	table = (t_table *)malloc(sizeof(t_table) * 1);
	if (!table)
		return (0);
	n_phils = 3;
	phils = (t_phil *)malloc(sizeof(t_phil) * n_phils);
	if (!phils)
		return (0);
	forks = malloc(sizeof(pthread_mutex_t) * n_phils);
	if (!forks)
		return (0);
	gettimeofday(&current_time, NULL);
	//  MAX MICROSECONDS: 999999
	i = 0;
	time_to_die = 3000000;
	while (i < n_phils)
	{
		phils[i].countdown_diying = time_to_die;
		phils[i].countdown_eating = 1000000;
		phils[i].countdown_sleeping = 1000000;
		phils[i].is_thinking = 1;
		phils[i].is_sleeping = 0;
		phils[i].is_eating = 0;
		phils[i].id = i;
		phils[i].last_check = current_time.tv_usec;
		pthread_mutex_init(&(forks[i]), NULL);
		i++;
	}
	dead = 0;
	while (!dead)
	{
		// pthread_create(&(phils[i].thread_id), NULL, ft_try_eat, phils[i])
		update(n_phils, phils);
		dead = ft_anyone_dead(n_phils, phils, time_to_die);
		// dead = 1;
	}
} */