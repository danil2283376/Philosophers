#include "philo_one.h"

int		check_count_eat(int j, t_global	*copy_global)
{
	if ((j == copy_global->arguments->quantity_philosophers) \
		&& copy_global->optional_varible)
	{
		pthread_mutex_lock(&copy_global->write_in_chat);
		return (ERROR_PHILOSOPHERS_ATE);
	}
	return (WORK_IS_FINE);
}

int		check_life_philosoph(int i, t_global	*copy_global)
{
	if (copy_global->arguments->time_to_die < \
		get_time(copy_global->philosophers[i].time_life))
	{
		pthread_mutex_lock(&copy_global->write_in_chat);
		printf("time %zu, Philosoph %d die\n", \
			get_time(copy_global->start_programm_time), i);
		return (ERROR_PHILOSOPH_DIE);
	}
	return (0);
}

void	*watcher_proccess(void *global)
{
	int			i;
	t_global	copy_global;
	int			j;

	i = 0;
	j = 0;
	copy_global = (*(t_global *)global);
	while (1)
	{
		while (i < copy_global.arguments->quantity_philosophers)
		{
			if (check_life_philosoph(i, &copy_global) == ERROR_PHILOSOPH_DIE)
				return (NULL);
			if ((copy_global.arguments->nbr_of_times_each_philo_must_eat \
				== copy_global.philosophers[i].count_eat)
				&& copy_global.optional_varible)
				++j;
			++i;
		}
		if (check_count_eat(j, &copy_global) == ERROR_PHILOSOPHERS_ATE)
			return (NULL);
		i = 0;
	}
	return (NULL);
}
