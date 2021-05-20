#include "philo_three.h"

void	value_philosoph(t_global *global, t_philosoph *philosoph, int number)
{
	philosoph->count_eat = 0;
	philosoph->start_programm_time = global->start_programm_time;
	philosoph->write_in_chat = global->write_in_chat;
	philosoph->time_to_die = global->arguments->time_to_die;
	philosoph->time_to_eat = global->arguments->time_to_eat;
	philosoph->time_to_sleep = global->arguments->time_to_sleep;
	philosoph->semaphors = global->semaphors;
	philosoph->number = number;
	philosoph->nbr_of_times_each_philo_must_eat = \
		global->arguments->nbr_of_times_each_philo_must_eat;
	philosoph->five_argument_exist = global->optional_varible;
	philosoph->other_philosoph = global->philosoph_pid;
	philosoph->count_philosophs = global->arguments->quantity_philosophers;
	philosoph->time_life = global->start_programm_time;
	philosoph->defender = global->defender;
}

void	philosopher_start(t_global *global, int number)
{
	t_philosoph	philosoph;
	pthread_t	t_check_life;
	pthread_t	t_check_count_eat;
	pthread_t	thread_philosoph;

	thread_philosoph = malloc(sizeof(pthread_t) * 1);
	t_check_count_eat = malloc(sizeof(pthread_t) * 1);
	t_check_life = malloc(sizeof(pthread_t) * 1);
	value_philosoph(global, &philosoph, number);
	pthread_create(&thread_philosoph, NULL, \
		start_philosoph, (void *)&philosoph);
	pthread_create(&t_check_count_eat, NULL, \
		check_count_eat, (void *)&philosoph);
	pthread_create(&t_check_life, NULL, check_life, (void *)&philosoph);
	pthread_join(t_check_life, NULL);
}

int	create_semophores(t_global *global)
{
	global->semaphors = sem_open("/semaphors", O_CREAT, \
		0666, global->arguments->quantity_philosophers);
	sem_unlink("/semaphors");
	global->ones_die = sem_open("/ones_die", O_CREAT, 0666, 1);
	sem_unlink("/ones_die");
	global->defender = sem_open("/defender", O_CREAT, 0666, 1);
	sem_unlink("/defender");
	global->write_in_chat = sem_open("/chat", O_CREAT, 0666, 1);
	sem_unlink("/chat");
	global->philosoph_pid = malloc(sizeof(pid_t) \
		* global->arguments->quantity_philosophers);
	if (global->semaphors == NULL || global->ones_die == NULL \
		|| global->defender == NULL || global->write_in_chat == 0)
		return (ERROR_CREATE_SIMOPHORE);
	return (WORK_IS_FINE);
}

void	create_philosopher(t_global *global)
{
	int	i;
	int	error;
	int	status;

	i = 0;
	status = 0;
	if (create_semophores(global) == ERROR_CREATE_SIMOPHORE)
		return ;
	while (i < global->arguments->quantity_philosophers)
	{
		global->philosoph_pid[i] = fork();
		if (global->philosoph_pid[i] == 0)
			philosopher_start(global, i);
		else
		{
			i++;
		}
	}
	i = 0;
	while (i < global->arguments->quantity_philosophers)
	{
		waitpid(global->philosoph_pid[i], &error, status);
		i++;
	}
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
			create_philosopher(&global);
		}
	}
}
