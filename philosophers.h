#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

# define DEATH_TOLL_MS 5
# define WAIT_FOR_FORKS_MS 1000
# define CHECK_EXIT_CYCLE_MS 500

typedef enum	e_state
{
	EATING,
	SLEEPING,
	THINKING,
	RUNNING,
	FINISHED
}				t_state;

typedef struct 	s_fork
{
	int				owner;
	int				used;
	pthread_mutex_t	f_access;
}				t_fork;

typedef struct	s_phil
{
	int				id;
	int				n_eat;
	int				eat_start;
	pthread_mutex_t	p_access;
	t_state			p_state;
	t_fork			fork;
	pthread_t		p_thread;
	struct s_table	*table;
}				t_phil;

typedef	struct 	s_table
{
	int				n_phils;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_meals;
	t_state			state;
	pthread_mutex_t	print_access;
	pthread_mutex_t	start_access;
	t_phil			*phils;
}				t_table;

int		ft_atoi(const char *str);
void	ft_putnbr(int nb);
void	ft_putstr(int fd, char *str);
int		ft_error(char *s, int error);
int		elapsed_time(void);
void	init_all(t_table *table);
int		ft_parsing(t_table *table, int argc, char *argv[]);
void	ft_display(t_phil *phil, int timestamp, char *msg);

#endif