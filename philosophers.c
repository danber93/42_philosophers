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
		if (phils[i].countdown_dying < time_to_die)
			printf("phils[%i] has %u milliseconds before dying\n", i, phils[i].countdown_dying);
		i++;
	}
	return (-1);
}

long	get_time_ms(void)
{
	// printf("sono in get_time_ms\n");
	struct timeval	tv;
	unsigned long	time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000;
	time += tv.tv_usec / 1000;
	return (time);
}

long	ft_get_time_diff(long int last_check)
{
	// printf("sono in ft_get_diff\n");
	return (get_time_ms() - last_check);
}

long	ft_diff_ms(unsigned int micros, long last_check)
{
	long ms;

	ms = (long)micros;
	if (ms < last_check)
		return ((long)(last_check + ms - 1000000));
	return ((long)(ms - last_check));
}

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
		// table->phils[i].last_check = current_time.tv_usec;
		table->phils[i].last_check = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
		// table->phils[i].last_check = get_time_ms();
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
	table->time_to_eat = ft_atoi(argv[3]) * 1000;
	table->time_to_sleep = ft_atoi(argv[4]) * 1000;
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
			// table->phils[i].countdown_dying -= ft_diff_ms(current_time.tv_usec, table->phils[i].last_check);
			table->phils[i].countdown_dying -= ft_get_time_diff(table->phils[i].last_check);
			current = i;
			next = current + 1;
			if (i == table->n_phils)
				next = 0;
			if (table->phils[current].is_eating == 0 && table->phils[next].is_eating == 0)
				return (current);
		}
		else if ((table->phils[i]).is_sleeping)
		{
			// table->phils[i].countdown_sleeping -= ft_diff_ms(current_time.tv_usec, table->phils[i].last_check);
			// table->phils[i].countdown_dying -= ft_diff_ms(current_time.tv_usec, table->phils[i].last_check);
			table->phils[i].countdown_sleeping -= ft_get_time_diff(table->phils[i].last_check);
			table->phils[i].countdown_dying -= ft_get_time_diff(table->phils[i].last_check);
			if (table->phils[i].countdown_sleeping <= 0)
			{
				table->phils[i].countdown_sleeping = 0;
				table->phils[i].is_sleeping = 0;
				table->phils[i].is_thinking = 1;
			}
		}
		table->phils[i].last_check = current_time.tv_usec;
		//printf("Phil %i has still %u milliseconds before dying.\n", i, table->phils[i].countdown_dying / 1000);
		// printf("Phil %i has still %u us before dying.\n", i,table->phils[i].countdown_dying);
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
		// printf("I NEED TO EAT AND I AM phil[%i]\n", current);
		next = current + 1;
		if (current == table->n_phils - 1)
			next = 0;
		table->phils[current].is_thinking = 0;
		table->phils[current].is_eating = 1;
		struct timeval now;
		gettimeofday(&now, NULL);
		printf("MY NAME IS phil[%i] AND I AM EATING AT: %u\n", current, now.tv_sec * 1000 + now.tv_usec / 1000);
		pthread_mutex_lock(&(table->forks[current]));
		pthread_mutex_lock(&(table->forks[next]));
		usleep(table->time_to_eat);
		// printf("HO FINITO DI MANGIARE AND I AM phils[%i]\n", current);
		// printf("SONO IL PHILS[%i] E I AM STARVING: %u\n", next, table->phils[next].countdown_dying);
		gettimeofday(&now, NULL);
		printf("SONO phil[%i] E HO FINITO DI MANGIARE A : %u. COUNTDOWN: %u\n", current, now.tv_sec * 1000 + now.tv_usec / 1000, table->phils[current].countdown_dying);
		table->phils[current].countdown_dying = table->time_to_die;
		// printf("init phils[%i]->countdown_dying = %u\n", current, table->phils[current].countdown_dying);
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
	while (dead < 0)
	{
		pthread_create(&(table->phils[i].thread_id), NULL, ft_eat, table);
		dead = ft_anyone_dead(table->n_phils, table->phils, table->time_to_die);
		if (i == table->n_phils)
			i = 0;
		else
			i++;
		dead = 1;
	}
	i = 0;
	while (i < table->n_phils)
			pthread_join((table->phils[i++].thread_id), NULL);
	usleep(2000);
	printf("Phils[%i] HAS DIED\n", dead);
}