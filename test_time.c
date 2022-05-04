#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_phil
{
	pthread_mutex_t m1, m2;
	int				value;
	long int		last_check;
}			t_phil;

void	*ft_fun(void *arg)
{
	t_phil *phil;
	struct timeval now;

	phil = (t_phil*)arg;
	gettimeofday(&now, NULL);
	printf("last check on phil: %ld milliseconds\n", phil->last_check);
	printf("before calling the mutex and sleep: %ld milliseconds\n", (now.tv_sec * 1000) + now.tv_usec / 1000);
	pthread_mutex_lock(&(phil->m1));
	pthread_mutex_lock(&(phil->m2));
	usleep(1000);
	phil->value = 1;
	gettimeofday(&now, NULL);
	printf("after calling the mutex and sleep: %ld milliseconds\n", (now.tv_sec * 1000) + now.tv_usec / 1000);
	pthread_mutex_unlock(&(phil->m1));
	pthread_mutex_unlock(&(phil->m2));
	return (NULL);
}

int	main(void)
{
	pthread_t p1, p2;
	t_phil *phil;
	int i;
	struct timeval current_time;

	phil = (t_phil *)malloc(sizeof(t_phil));
	phil->value = 0;
	gettimeofday(&current_time, NULL);
	phil->last_check = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	pthread_mutex_init(&(phil->m1), NULL);
	pthread_mutex_init(&(phil->m2), NULL);
	pthread_create(&p1, NULL, ft_fun, (void *)phil);
	while (phil->value == 0)
		i++;
	// pthread_join(p1, NULL);
	return (0);
}