#include "../libft/libft.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <semaphore.h>
#include <signal.h>

#define WORK_IS_FINE					0
#define ERROR_QUANTITY_PHILOSOPHERS 	1
#define ERROR_TIME_TO_DIE				2
#define ERROR_TIME_TO_EAT				3
#define ERROR_TIME_TO_SLEEP				4
#define ERROR_QUANTITY_PHILOSOPHER_EAT	5
#define ERROR_PHILOSOPH_DIE				6
#define ERROR_PHILOSOPHERS_ATE			7

#pragma region structs

typedef struct		s_arguments
{
	int				quantity_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_times_each_philo_must_eat;
}					t_arguments;

typedef struct		s_philosoph
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				count_eat;
	int				number;
	ssize_t			time_life;
	sem_t           *write_in_chat;
	ssize_t			start_programm_time;
    sem_t           *semaphors;
    int             quantity_philo_start;
    int             quantity_philosoph;
	int				nbr_of_times_each_philo_must_eat;
	int				five_argument_exist;
	sem_t			*ones_die;
	pid_t			*other_philosoph;
	int				count_philosophs;
}					t_philosoph;

typedef struct			s_global
{
	t_arguments			*arguments;
    sem_t               *semaphors;
	ssize_t				start_programm_time;
	int					status_working;
	sem_t               *write_in_chat;
	int 				optional_varible;
	sem_t				*ones_die;
	pid_t				*philosoph_pid;
}						t_global;
#pragma endregion

#pragma region addtional_function

void    my_usleep(ssize_t time)
{
    struct timeval  t1;
    ssize_t         timing;

    gettimeofday(&t1, NULL);
    timing = t1.tv_sec * 1000 + t1.tv_usec / 1000;
    while (time > (t1.tv_sec * 1000 + t1.tv_usec / 1000) - timing)
    {
        gettimeofday(&t1, NULL);
        usleep(100);
    }
}

ssize_t get_time(ssize_t begin_time)
{
    struct timeval  t1;

    gettimeofday(&t1, NULL);
    return ((t1.tv_sec * 1000 + t1.tv_usec / 1000) - begin_time);
}

int	error(char *message) 
{
	printf("%s\n", message);
	return(1);
}

#pragma endregion

#pragma region parsing_arguments
int		fill_struct(int argc, char **argv, t_arguments *arguments) 
{
	int number;

	number = ft_atoi(argv[1]); // число философов
	if (number < 1 || number > 200)
		return (ERROR_QUANTITY_PHILOSOPHERS);
	arguments->quantity_philosophers = number;
	number = ft_atoi(argv[2]); // время смерти каждого философова
	if (number < 1)
		return (ERROR_TIME_TO_DIE);
	arguments->time_to_die = number;
	number = ft_atoi(argv[3]); // время для еды
	if (number < 1)
		return (ERROR_TIME_TO_EAT);
	arguments->time_to_eat = number;
	number = ft_atoi(argv[4]); // время для сна
	if (number < 1)
		return (ERROR_TIME_TO_SLEEP);
	arguments->time_to_sleep = number;
	if (argc == 6)
	{
		number = ft_atoi(argv[5]); // сколько каждый философ должен есть
		if (number < 1)
			return (ERROR_QUANTITY_PHILOSOPHER_EAT);
		arguments->nbr_of_times_each_philo_must_eat = number;
	}
	return (WORK_IS_FINE);
}
#pragma endregion

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
	philosoph->nbr_of_times_each_philo_must_eat = global->arguments->nbr_of_times_each_philo_must_eat;
	philosoph->five_argument_exist = global->optional_varible;
	philosoph->other_philosoph = global->philosoph_pid;
	philosoph->count_philosophs = global->arguments->quantity_philosophers;
	philosoph->time_life = global->start_programm_time;//get_time(0);
}

void	philosoph_eat(t_philosoph *philosoph, int number)
{
	sem_wait(philosoph->semaphors);
	sem_wait(philosoph->write_in_chat);
	printf("time %zu, Take RIGHT fork %d!\n", get_time(philosoph->start_programm_time), number);
	sem_post(philosoph->write_in_chat);
	sem_wait(philosoph->semaphors);
	sem_wait(philosoph->write_in_chat);
    printf("time %zu, Take LEFT fork %d!\n", get_time(philosoph->start_programm_time), number);
	sem_post(philosoph->write_in_chat);
    sem_wait(philosoph->write_in_chat);
    philosoph->time_life = get_time(0);
    printf("time %zu, Philosoph number: %d, eat!\n", get_time(philosoph->start_programm_time), number);
	++philosoph->count_eat;
	sem_post(philosoph->write_in_chat);
	my_usleep(philosoph->time_to_eat);
	sem_post(philosoph->semaphors);
    sem_post(philosoph->semaphors);
}

void	philosoph_sleep(t_philosoph *philosoph, int number)
{
	sem_wait(philosoph->write_in_chat);
	printf("time %zu, Philosoph number %d sleeping!\n", get_time(philosoph->start_programm_time), number);
	sem_post(philosoph->write_in_chat);
    my_usleep(philosoph->time_to_sleep);
}

void	philosoph_thinking(t_philosoph *philosoph, int number)
{
	sem_wait(philosoph->write_in_chat);
    printf("time %zu, Philosoph number %d thinking!\n", get_time(philosoph->start_programm_time), number);
    sem_post(philosoph->write_in_chat);
}

void	*start_philosoph(void *philosoph) 
{
    t_philosoph *copy_philosoph;

    copy_philosoph = (t_philosoph *)philosoph;
    copy_philosoph->time_life = get_time(0);
    while (1)
    {
        philosoph_eat(copy_philosoph, copy_philosoph->number);
        philosoph_sleep(copy_philosoph, copy_philosoph->number);
        philosoph_thinking(copy_philosoph, copy_philosoph->number);
    }
	return (NULL);
}

void	*check_life(void *philosoph)
{
	t_philosoph *copy_philosoph;

	copy_philosoph = (t_philosoph *)philosoph;
	while (1)
	{
		usleep(copy_philosoph->time_to_die / 4);
		// printf("time_to_die: %d < %zd\n", copy_philosoph->time_to_die, get_time(copy_philosoph->time_life));
		if (copy_philosoph->time_to_die < get_time(copy_philosoph->time_life))
		{
			// printf("AAAAAA\n");
			// sem_wait(copy_philosoph->write_in_chat);
			printf("time_to_die: %d < %zd\n", copy_philosoph->time_to_die, get_time(copy_philosoph->time_life));
			int i;

			// i = 0;
			// printf("time %zu, Philosoph %d die\n", get_time(copy_philosoph->start_programm_time), copy_philosoph->number);
			while (i < copy_philosoph->count_philosophs)
			{
				kill(copy_philosoph->other_philosoph[i], 2);
				i++;
			}
			// exit(1);
		}
	}
	return (NULL);
}

void	*check_count_eat(void *philosoph) 
{
	t_philosoph *copy_philosoph;

	copy_philosoph = (t_philosoph *)philosoph;
	if (copy_philosoph->five_argument_exist == 1)
	{
		while (1)
		{
			if (copy_philosoph->count_eat > copy_philosoph->nbr_of_times_each_philo_must_eat)
			{
				printf("%d > %d\n", copy_philosoph->count_eat, copy_philosoph->nbr_of_times_each_philo_must_eat);
				printf("COACAT\n");
				exit(0);
			}
		}
	}
	return (NULL);
}

void	philosopher_start(t_global *global, int number)
{
	t_philosoph philosoph;
	pthread_t t_check_life = malloc(sizeof(pthread_t) * 1);
	pthread_t t_check_count_eat = malloc(sizeof(pthread_t) * 1);
	pthread_t thread_philosoph = malloc(sizeof(pthread_t) * 1);

	value_philosoph(global, &philosoph, number);
	pthread_create(&thread_philosoph, NULL, start_philosoph, (void *)&philosoph);
	pthread_create(&t_check_count_eat, NULL, check_count_eat, (void *)&philosoph);
	pthread_create(&t_check_life, NULL, check_life, (void *)&philosoph);
	pthread_join(t_check_life, NULL);
}

void    create_philosopher(t_global *global)
{
    global->semaphors = sem_open("/semaphors", O_CREAT, 0666, global->arguments->quantity_philosophers);
    sem_unlink("/semaphors");
    global->philosoph_pid = malloc(sizeof(pid_t) * global->arguments->quantity_philosophers);
    sem_t *chat = sem_open("/chat", O_CREAT, 0666, 1);
    sem_unlink("/chat");
	global->ones_die = sem_open("/ones_die", O_CREAT, 0666, 1);
	sem_unlink("/ones_die");
    int i;

    i = 0;
	global->write_in_chat = chat;
    while (i < global->arguments->quantity_philosophers)
    {
		global->philosoph_pid[i] = fork();
		if (global->philosoph_pid[i] == 0)
		{
			philosopher_start(global, i);
			break ;
		}
		else
			i++;
    }
	int error;
	int status;

	i = 0;
	while (i < global->arguments->quantity_philosophers)
	{
		waitpid(global->philosoph_pid[i], &error, status);
		i++;
	}
}

int main(int argc, char **argv)
{
    t_arguments arguments;
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
		printf("%d %d %d %d %d\n", arguments.quantity_philosophers, 
		arguments.time_to_die, arguments.time_to_eat, arguments.time_to_sleep, 
		arguments.nbr_of_times_each_philo_must_eat);
		global.arguments = &arguments;
		if (check_status != 0)
			return (error("Error: arguments not fine!"));
		else
		{
			create_philosopher(&global);
		}
	}
}