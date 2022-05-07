#include "philosophers.h"

int	take_forks(t_phil *phil, t_phil *next)
{
	int	ready;

	ready = 0;
	if (!next)
		return ready;
	pthread_mutex_lock(&phil->fork.f_access);
	pthread_mutex_lock(&next->fork.f_access);
	if ((phil->fork.owner != phil->id && !phil->fork.used)
		&& (next->fork.owner != phil->id && !next->fork.used))
	{
		phil->fork.owner = phil->id;
		next->fork.owner = phil->id;
		phil->fork.used = 1;
		next->fork.used = 1;
		ready = 1;
	}
	pthread_mutex_unlock(&phil->fork.f_access);
	pthread_mutex_unlock(&next->fork.f_access);
	return (ready);
}

void	drop_forks(t_phil *phil, t_phil *next)
{
	pthread_mutex_lock(&phil->fork.f_access);
	pthread_mutex_lock(&next->fork.f_access);
	phil->fork.used = 0;
	next->fork.used = 0;
	pthread_mutex_unlock(&phil->fork.f_access);
	pthread_mutex_unlock(&next->fork.f_access);
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
		usleep(WAIT_FOR_FORKS_MS);
	phil->p_state = EATING;
}

void	ft_eat(t_phil *phil, t_phil* next)
{
	pthread_mutex_lock(&phil->p_access);
	ft_display(phil, elapsed_time(),
		"has taken a fork|has taken a fork|is eating");
	phil->n_eat++;
	phil->eat_start = elapsed_time();
	pthread_mutex_unlock(&phil->p_access);
	usleep(phil->table->time_to_eat * 1000);
	drop_forks(phil, next);
	phil->p_state = SLEEPING;
}