#include "philosophers.h"

static long	long	ft_get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	elapsed_time(void)
{
	static unsigned long	start = 0;

	if (!start)
		start = ft_get_time();
	return ((ft_get_time() - start));
}
