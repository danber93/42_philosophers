#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	i;

void	*test_mutex_lock(void *arg)
{
	pthread_mutex_t *m;
	int res;

	printf("Sono dentro la funzione\n");
	m = (pthread_mutex_t *)arg;
	res = pthread_mutex_lock(m);
	printf("res = %i\n", res);
	return (NULL);
}

int	main()
{
	pthread_t	p1, p2;
	pthread_mutex_t m1, m2;
	pthread_mutex_t *m;

	i = 0;
	m = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * 2);
	pthread_mutex_init(&(m[0]), NULL);
	pthread_mutex_init(&(m[1]), NULL);

	printf("sono qui\n");
	pthread_create(&p1, NULL, test_mutex_lock, &(m[0]));
	pthread_create(&p2, NULL, test_mutex_lock, &(m[0]));
	usleep(500000000);
	pthread_mutex_unlock(&m[0]);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	printf("ho finito\n");
}