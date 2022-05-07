#include "philosophers.h"

int	take_forks(t_phil *phil, t_phil *next)
{
	int	ready;

	ready = 0;
	if (!next)
		return ready;
	pthread_mutex_lock(&phil->p_fork.f_access);
	pthread_mutex_lock(&next->p_fork.f_access);
	if ((phil->p_fork.owner != phil->id && !phil->p_fork.used)
		&& (next->p_fork.owner != phil->id && !next->p_fork.used))
	{
		phil->p_fork.owner = phil->id;
		next->p_fork.owner = phil->id;
		phil->p_fork.used = 1;
		next->p_fork.used = 1;
		ready = 1;
	}
	pthread_mutex_unlock(&phil->p_fork.f_access);
	pthread_mutex_unlock(&next->p_fork.f_access);
	return (ready);
}

void	drop_forks(t_phil *phil, t_phil *next)
{
	pthread_mutex_lock(&phil->p_fork.f_access);
	pthread_mutex_lock(&next->p_fork.f_access);
	phil->p_fork.used = 0;
	next->p_fork.used = 0;
	pthread_mutex_unlock(&phil->p_fork.f_access);
	pthread_mutex_unlock(&next->p_fork.f_access);
}

void	ft_sleep(t_phil *phil)
{
	ft_display(phil, elapsed_time(), "is sleeping.");
	usleep(phil->table->time_to_sleep * 1000);
	phil->p_state = THINKING;
}

void	ft_think(t_phil *phil, t_phil *next)
{
	ft_display(phil, elapsed_time(), "is thinking");
	while (phil->table->state == RUNNING && !take_forks(phil, next))
		usleep(1000);
	phil->p_state = EATING;
}

void	ft_eat(t_phil *phil, t_phil* next)
{
	pthread_mutex_lock(&phil->p_access);
	
}