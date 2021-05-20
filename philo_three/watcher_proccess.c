#include "philo_three.h"

void	*check_life(void *philosoph)
{
	t_philosoph	*copy_philosoph;
	int			i;

	copy_philosoph = (t_philosoph *)philosoph;
	i = 0;
	while (1)
	{
		usleep(copy_philosoph->time_to_die / 4);
		if (copy_philosoph->time_to_die < get_time(copy_philosoph->time_life))
		{
			printf("time %zu, Philosoph %d die\n", \
				get_time(copy_philosoph->start_programm_time), \
					copy_philosoph->number);
			while (i < copy_philosoph->count_philosophs)
			{
				kill(copy_philosoph->other_philosoph[i], 2);
				i++;
			}
		}
	}
	return (NULL);
}

void	*check_count_eat(void *philosoph)
{
	int			i;
	t_philosoph	*copy_philosoph;

	copy_philosoph = (t_philosoph *)philosoph;
	i = 0;
	if (copy_philosoph->five_argument_exist == 1)
	{
		while (1)
		{
			if (copy_philosoph->count_eat \
				> copy_philosoph->nbr_of_times_each_philo_must_eat)
			{
				while (i < copy_philosoph->count_philosophs)
				{
					kill(copy_philosoph->other_philosoph[i], 2);
					i++;
				}
				exit(0);
			}
		}
	}
	return (NULL);
}
