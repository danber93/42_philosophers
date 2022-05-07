#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int result;
	int sign;

	sign = 1;
	result = 0;
	while (*str && (*str == '\r' || *str == '\n' || *str == '\t'
	|| *str == '\v' || *str == '\f' || *str == ' '))
		str++;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (!(*str >= 48 && *str <= 57))
			return (-1);
		result = result * 10 + *str - 48;
		str++;
	}
	return (result * sign);
}

void			ft_putnbr(int nb)
{
	char	c;

	if (nb / 10)
		ft_putnbr(nb / 10);
	c = nb % 10 + '0';
	write(1, &c, 1);
}

void			ft_putstr(int fd, char *str)
{
	while (*str)
		write(fd, str++, 1);
}

int				ft_error(char *s, int error)
{
	ft_putstr(2, s);
	ft_putstr(2, "\n");
	return (error);
}

void	ft_display(t_phil *phil, int timestamp, char *msg)
{
	static	int	last_timestamp = 0;

	pthread_mutex_lock(&(phil->table->print_access));
	if (last_timestamp > timestamp)
		timestamp = last_timestamp;
	last_timestamp = timestamp;
	if (phil->table->state != RUNNING)
	{
		pthread_mutex_unlock(&(phil->table->print_access));
		return ;
	}
	while (*msg)
	{
		ft_putnbr(timestamp);
		ft_putstr(1, " ");
		ft_putnbr(phil->id + 1);
		ft_putstr(1, " ");
		while (*msg && *msg != '|')
			write(1, msg++, 1);
		ft_putstr(1, "\n");
		if (*msg)
			++msg;
	}
	pthread_mutex_unlock(&(phil->table->print_access));
}