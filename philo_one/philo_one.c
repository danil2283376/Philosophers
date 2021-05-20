#include "philo_one.h"

void	value_philosoph(t_global *global, int i,
	pthread_mutex_t chat, pthread_t *philosophs)
{
	global->philosophers[i].time_to_die = global->arguments->time_to_die;
	global->philosophers[i].time_to_eat = global->arguments->time_to_eat;
	global->philosophers[i].time_to_sleep = global->arguments->time_to_sleep;
	global->philosophers[i].number = i;
	global->philosophers[i].left = &global->mutexs[i];
	global->philosophers[i].right = &global->mutexs[(i + 1)
		% global->arguments->quantity_philosophers];
	global->philosophers[i].time_life = global->start_programm_time;
	global->philosophers[i].start_programm_time = global->start_programm_time;
	global->philosophers[i].write_in_chat = &chat;
	global->philosophers[i].name_flow = philosophs[i];
	global->philosophers[i].count_eat = 0;
}

void	wait_threads(t_global *global, pthread_t *philosophs)
{
	pthread_t		watcher;

	pthread_create(&watcher, NULL, watcher_proccess, (void *)global);
	pthread_join(watcher, NULL);
	pthread_detach(watcher);
	free(philosophs);
	free(global->philosophers);
	free(global->mutexs);
}

void	philosopher_start(t_global *global)
{
	pthread_t		*philosophs;
	int				i;

	philosophs = malloc((global->arguments->quantity_philosophers)
			* sizeof(pthread_t));
	global->philosophers = malloc(sizeof(t_philosoph) \
			* global->arguments->quantity_philosophers);
	global->mutexs = malloc(sizeof(pthread_mutex_t) \
		* global->arguments->quantity_philosophers);
	i = 0;
	while (i < global->arguments->quantity_philosophers)
	{
		pthread_mutex_init(&global->mutexs[i], NULL);
		++i;
	}
	pthread_mutex_init(&global->write_in_chat, NULL);
	i = 0;
	while (i < global->arguments->quantity_philosophers)
	{
		value_philosoph(global, i, global->write_in_chat, philosophs);
		pthread_create(&philosophs[i], NULL, proccess_philosoph,
			(void *)(&global->philosophers[i]));
		++i;
	}
	wait_threads(global, philosophs);
}

int	main(int argc, char **argv)
{
	t_arguments	arguments;
	t_global	global;
	int			check_status;

	global.start_programm_time = get_time(0);
	if (argc != 6 && argc != 5)
		error("Error: arguments!");
	else
	{
		if (argc == 6)
			global.optional_varible = 1;
		else
			global.optional_varible = 0;
		check_status = fill_struct(argc, argv, &arguments);
		global.arguments = &arguments;
		if (check_status != 0)
			return (error("Error: arguments not fine!"));
		else
		{
			philosopher_start(&global);
		}
	}
}
