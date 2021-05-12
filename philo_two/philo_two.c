#include "../libft/libft.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <semaphore.h>

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
	int				number;
	int				count_eat;
	ssize_t			time_life;
	sem_t           *write_in_chat;
	ssize_t			start_programm_time;
	pthread_t		name_thread;
    sem_t           *semaphors;
    sem_t           *waiter;
    sem_t           *security;
}					t_philosoph;

typedef struct			s_global
{
	t_arguments			*arguments;
	t_philosoph			*philosophers;
    sem_t               *semaphors;
	ssize_t				start_programm_time;
	int					status_working;
	sem_t               *write_in_chat;
	int 				optional_varible;
    sem_t               *waiter;
    sem_t               *security;
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

void    philosoph_eat(t_philosoph *philosoph) 
{
    sem_wait(philosoph->waiter);
    sem_wait(philosoph->semaphors);
    sem_wait(philosoph->write_in_chat);
    printf("time %zu, Take RIGHT fork %d!\n", get_time(philosoph->start_programm_time), philosoph->number);
    sem_post(philosoph->write_in_chat);
    sem_wait(philosoph->semaphors);
    sem_wait(philosoph->write_in_chat);
    printf("time %zu, Take LEFT fork %d!\n", get_time(philosoph->start_programm_time), philosoph->number);
    sem_post(philosoph->waiter);
    sem_post(philosoph->write_in_chat);
    sem_wait(philosoph->write_in_chat);
    printf("time %zu, Philosoph number: %d, eat!\n", get_time(philosoph->start_programm_time), philosoph->number);
    ++philosoph->count_eat;
    // printf("time_life: %zu\n", get_time(philosoph->time_life));
    sem_post(philosoph->write_in_chat);
    philosoph->time_life = get_time(0);
    my_usleep(philosoph->time_to_eat);
    sem_post(philosoph->semaphors);
    sem_post(philosoph->semaphors);
}

void    philosoph_sleep(t_philosoph *philosoph)
{
    sem_wait(philosoph->write_in_chat);
    printf("time %zu, Philosoph number %d sleeping!\n", get_time(philosoph->start_programm_time), philosoph->number);
    sem_post(philosoph->write_in_chat);
    my_usleep(philosoph->time_to_sleep);
}

void    philosoph_thinking(t_philosoph *philosoph) 
{
    sem_wait(philosoph->write_in_chat);
    printf("time %zu, Philosoph number %d thinking!\n", get_time(philosoph->start_programm_time), philosoph->number);
    sem_post(philosoph->write_in_chat);
}

void    *proccess_philosoph(void *philosoph)
{
    pthread_detach((*(t_philosoph *)philosoph).name_thread);
    t_philosoph *copy_philosoph;

    copy_philosoph = (t_philosoph *)philosoph;
    copy_philosoph->time_life = get_time(0);
    printf("\ntime_life: %zu\n", get_time(copy_philosoph->time_life));
    while (1)
    {
        philosoph_eat(copy_philosoph);
        philosoph_sleep(copy_philosoph);
        philosoph_thinking(copy_philosoph);
        sem_post(copy_philosoph->security);
        // printf("time_life: %zu\n", get_time(copy_philosoph->time_life));
    }
}

void	value_philosoph(t_global *global, int i, sem_t *chat, pthread_t *philosophs) 
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
    global->philosophers[i].count_eat = 0;
    global->philosophers[i].security = global->security;
}

// ДОБАВИТЬ ОФИЦИАНТА

void	*watcher_proccess(void *global)
{
    t_global *copy_global;
    int i;
    int j;

    i = 0;
    j = 0;
    copy_global = (t_global *)global;
    while (1) 
    {
        while (i < copy_global->arguments->quantity_philosophers)
        {
            if (copy_global->arguments->time_to_die < get_time(copy_global->philosophers[i].time_life))
            {
                sem_wait(copy_global->write_in_chat);
                printf("time %zu, Philosoph %d die\n", get_time(copy_global->start_programm_time), i);
                return (NULL);
            }
            if ((copy_global->arguments->nbr_of_times_each_philo_must_eat == copy_global->philosophers[i].count_eat)
				&& copy_global->optional_varible)
                ++j;
            ++i;
        }
        if ((j == copy_global->arguments->quantity_philosophers) && copy_global->optional_varible)
        {
            sem_wait(copy_global->write_in_chat);
            return (NULL);
        }
        i = 0;
    }
    return (NULL);
}

void    philosopher_start(t_global *global) 
{
    pthread_t *philosophs = malloc(sizeof(pthread_t) * global->arguments->quantity_philosophers);
    global->philosophers = malloc(sizeof(t_philosoph) * global->arguments->quantity_philosophers);
    global->semaphors = sem_open("/semaphors", O_CREAT, 0666, global->arguments->quantity_philosophers);
    sem_unlink("/semaphor");
    int i;
    pthread_t watcher;

    global->write_in_chat = sem_open("/chat", O_CREAT, 0666, 1);
    sem_unlink("/chat");
    global->waiter = sem_open("/waiter", O_CREAT, 0666, 1);
    sem_unlink("/waiter");
    global->security = sem_open("/security", O_CREAT, 0666, 1);
    sem_unlink("/security");
    // printf("HELL\n");
    if (global->semaphors == NULL || global->write_in_chat == NULL || global->waiter == NULL || global->security == NULL)
        return ;
    i = 0;
    while (i < global->arguments->quantity_philosophers)
    {
        sem_wait(global->security);
        value_philosoph(global, i, global->write_in_chat, philosophs);
        pthread_create(&philosophs[i], NULL, proccess_philosoph, (void *)(&global->philosophers[i]));
        ++i;
    }
    pthread_create(&watcher, NULL, watcher_proccess, (void *)global);
    pthread_join(watcher, NULL);
    // i = 0;
    // while (i < global->arguments->quantity_philosophers) 
    // {
    //     pthread_join(philosophs[i], NULL);
    //     i++;
    // }
    // write(1, "HUI\n", 4);
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
			philosopher_start(&global);
		}
	}
}