#include "philo_two.h"

void	value_philosoph(t_global *global,
	int i, sem_t *chat, pthread_t *philosophs)
{
	global->philosophers[i].time_to_die = global->arguments->time_to_die;
	global->philosophers[i].time_to_eat = global->arguments->time_to_eat;
	global->philosophers[i].time_to_sleep = global->arguments->time_to_sleep;
	global->philosophers[i].number = i;
	global->philosophers[i].time_life = global->start_programm_time;
	global->philosophers[i].start_programm_time = global->start_programm_time;
	global->philosophers[i].write_in_chat = chat;
	global->philosophers[i].name_thread = philosophs[i];
	global->philosophers[i].waiter = global->waiter;
	global->philosophers[i].security = global->security;
	global->philosophers[i].count_eat = 0;
	global->philosophers[i].security = global->security;
}

int	create_semaphors(t_global *global)
{
	global->semaphors = sem_open("/semaphors", O_CREAT, \
		0666, global->arguments->quantity_philosophers);
	sem_unlink("/semaphor");
	global->write_in_chat = sem_open("/chat", O_CREAT, 0666, 1);
	sem_unlink("/chat");
	global->waiter = sem_open("/waiter", O_CREAT, 0666, 1);
	sem_unlink("/waiter");
	global->security = sem_open("/security", O_CREAT, 0666, 1);
	sem_unlink("/security");
	if (global->semaphors == NULL || global->write_in_chat == NULL \
		|| global->waiter == NULL || global->security == NULL)
		return (ERROR_CREATE_SIMOPHORE);
	return (WORK_IS_FINE);
}

void	philosopher_start(t_global *global)
{
	pthread_t	*philosophs;
	pthread_t	watcher;
	int			i;

	philosophs = malloc(sizeof(pthread_t) \
		* global->arguments->quantity_philosophers);
	global->philosophers = malloc(sizeof(t_philosoph) \
		* global->arguments->quantity_philosophers);
	if (create_semaphors(global) == ERROR_CREATE_SIMOPHORE)
		return ;
	i = 0;
	while (i < global->arguments->quantity_philosophers)
	{
		value_philosoph(global, i, global->write_in_chat, philosophs);
		pthread_create(&philosophs[i], NULL, proccess_philosoph, \
			(void *)(&global->philosophers[i]));
		++i;
	}
	pthread_create(&watcher, NULL, watcher_proccess, (void *)global);
	pthread_join(watcher, NULL);
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
