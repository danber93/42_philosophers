#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <pthread.h>

typedef struct s_table
{
	int n_philosophers;
	int n_forks;
	int *forks;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int n_meals;
}               t_table;

/*
ciclo
	p0 mangia (fh, fa)
		fa
	p1 aspetta
		fb
	p2 mangia (fb, fc)
		fc
	p3 aspetta
		fd
	p4 mangia (fd, fe)
		fe
	p5 aspetta
		ff
	p6 mangia (ff, fg)
		fg
	p7 aspetta (offset + time_to_eat < time_to_die)
		fh

ciclo
	p0 mangia (fe, fa)
		fa
	p1 aspetta
		fb
	p2 mangia (fb, fc)
		fc
	p3 aspetta
		fd
	p4 aspetta (offset + 2 * time_to_eat + [time_to_sleep]< time_to_die)
		fe

	usleep()

	controlla chi sta morendo di fame
		controllo se le sue forchette son lockate
		fallo mangiare


*/


/*
UPDATE
	ciclo
		diminuisci tutti i countdown
		corrent - last_meal_time > time_to_eat + time_to_sleep allora puoi mangiare
		current - last_meal_time > time_to_die E' MORTO
*/
typedef struct s_philosopher
{
	pthread_t phil_threah_id;
	int	phil_id;
	unsigned int countdown_starving;
	unsigned int last_meal_time; //il millisecondo in cui ha smesso di mangiare
	unsigned int last_check; // l'ultimo "time" in cui ho controllato nel ciclo UPDATE
	unsigned int countdown_sleeping;
	int is_eating;
	int is_sleeping;
	int is_thinking;
	int n_eaten;
}              t_philosopher;

#endif