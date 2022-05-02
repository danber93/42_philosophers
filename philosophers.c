#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct	s_phil
{
	long			countdown_eating;
	long			countdown_dying;
	long			countdown_sleeping;
	int				is_eating;
	int				is_sleeping;
	int				is_thinking;
	long			last_check;
	pthread_t		thread_id;
	int				id;
	int				n_meals;
}				t_phil;

typedef struct	s_table
{
	long			time_to_die;
	long			time_to_sleep;
	long			time_to_eat;
	int				n_phils;
	int				n_meals;
	t_phil			*phils;
	pthread_mutex_t	*forks;
}				t_table;

long	ft_atoi(char *str)
{
	int				i;
	long			res;

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

int		ft_anyone_dead(int n_phils, t_phil *phils, long time_to_die)
{
	int	i;

	i = 0;
	while (i < n_phils)
	{
		if (phils[i].countdown_dying <= 0) // || phils[i].countdown_diying > time_to_die)
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

long	ft_diff_ms(unsigned int micros, long last_check)
{
	long ms;

	ms = (long)micros;
	if (ms < last_check)
		return ((long)(last_check + ms - 1000000));
	return ((long)(ms - last_check));
}

// void	update_ct(int	n_phils, t_phil *phils)
// {
// 	struct timeval	current_time;
// 	int				i;

// 	i = 0;
// 	gettimeofday(&current_time, NULL);
// 	while (i < n_phils)
// 	{
// 		if ((phils[i]).is_eating)
// 		{
// 			phils[i].countdown_eating -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
// 		}
// 		if ((phils[i]).is_sleeping)
// 		{
// 			phils[i].countdown_sleeping -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
// 			phils[i].countdown_diying -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
// 			if (phils[i].countdown_sleeping <= 0)
// 				phils[i].countdown_sleeping = 0;
// 		}
// 		if ((phils[i]).is_thinking)
// 		{
// 			phils[i].countdown_diying -= ft_diff_ms(current_time.tv_usec, phils[i].last_check);
// 		}
// 		phils[i].last_check = current_time.tv_usec;
// 		// printf("Phil %i has still %u milliseconds before dying.\n", i, phils[i].countdown_diying / 1000);
// 		i++;
// 	}
// }

void	ft_init_phils_forks(t_table *table)
{
	int	i;
	struct timeval current_time;

	i = 0;
	while (i < table->n_phils)
	{
		gettimeofday(&current_time, NULL);
		table->phils[i].countdown_dying = table->time_to_die;
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
	table->phils = (t_phil *)malloc(sizeof(t_phil) * (table->n_phils));
	if (!(table->phils))
		return (NULL);
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * argc);
	if (!(table->forks))
		return (NULL);
	table->time_to_die = ft_atoi(argv[2]) * 1000;
	printf("time_to_die = %u\n", table->time_to_die);
	table->time_to_eat = ft_atoi(argv[3]) * 1000;
	printf("time_to_eat = %u\n", table->time_to_eat);
	table->time_to_sleep = ft_atoi(argv[4]) * 1000;
	printf("time_to_sleep = %u\n", table->time_to_sleep);
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

int	ft_need_to_eat(t_table *table)
{
	struct timeval	current_time;
	int				i;
	int				current;
	int				next;

	i = 0;
	gettimeofday(&current_time, NULL);
	while (i < table->n_phils)
	{
		if ((table->phils[i]).is_thinking)
		{
			printf("phils[%i].is_dying before update = %u\n", i, table->phils[i].countdown_dying);
			table->phils[i].countdown_dying -= ft_diff_ms(current_time.tv_usec, table->phils[i].last_check);
			printf("phils[%i].is_dying after update = %u\n", i, table->phils[i].countdown_dying);
			current = i;
			next = current + 1;
			if (i == table->n_phils)
				next = 0;
			if (table->phils[current].is_eating == 0 && table->phils[next].is_eating == 0)
				return (current);
		}
		else if ((table->phils[i]).is_sleeping)
		{
			table->phils[i].countdown_sleeping -= ft_diff_ms(current_time.tv_usec, table->phils[i].last_check);
			table->phils[i].countdown_dying -= ft_diff_ms(current_time.tv_usec, table->phils[i].last_check);
			if (table->phils[i].countdown_sleeping <= 0)
			{
				table->phils[i].countdown_sleeping = 0;
				table->phils[i].is_sleeping = 0;
				table->phils[i].is_thinking = 1;
			}
		}
		table->phils[i].last_check = current_time.tv_usec;
		// printf("Phil %i has still %u milliseconds before dying.\n", i,table->phils[i].countdown_diying / 1000);
		printf("Phil %i has still %u us before dying.\n", i,table->phils[i].countdown_dying);
		i++;
	}
	return (-1);
}

void	*ft_eat(void *arg)
{
	t_table *table;
	int	current;
	int	next;

	table = (t_table *)arg;
	current = ft_need_to_eat(table);
	if (current != -1)
	{
		printf("I NEED TO EAT AND I AM phil[%i]\n", current);
		next = current + 1;
		if (current == table->n_phils - 1)
			next = 0;
		table->phils[current].is_thinking = 0;
		table->phils[current].is_eating = 1;
		pthread_mutex_lock(&(table->forks[current]));
		pthread_mutex_lock(&(table->forks[next]));
		usleep(table->time_to_eat * 10);
		printf("HO FINITO DI MANGIARE AND I AM phils[%i]\n", current);
		printf("SONO IL PHILS[%i] E I AM STARVING: %u\n", next, table->phils[next].countdown_dying);
		table->phils[current].countdown_dying = table->time_to_die;
		table->phils[current].countdown_sleeping = table->time_to_sleep;
		pthread_mutex_unlock(&(table->forks[current]));
		pthread_mutex_unlock(&(table->forks[next]));
		table->phils[current].is_eating = 0;
		table->phils[current].is_sleeping = 1;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int		dead;
	int		i;
	t_table	*table;

	if (!(ft_check_input(argc, argv)))
		return (0);
	table = ft_init_table(argc, argv);
	dead = -1;
	i = 0;
	printf("i = %i\n", i);
	while (dead < 0)
	{
		pthread_create(&(table->phils[i].thread_id), NULL, ft_eat, table);
		// update_ct(table->n_phils, (table->phils));
		dead = ft_anyone_dead(table->n_phils, table->phils, table->time_to_die);
		if (i == table->n_phils)
		{
			i = 0;
			printf("NO ONE DIED\n");
		}
		else
			i++;
		// dead = 1;
	}
	i = 0;
	while (i < table->n_phils)
			pthread_join((table->phils[i++].thread_id), NULL);
	usleep(2000);
	printf("Phils[%i] HAS DIED\n", dead);
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