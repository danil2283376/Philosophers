#include "../libft/libft.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#define WORK_IS_FINE					0
#define ERROR_QUANTITY_PHILOSOPHERS 	1
#define ERROR_TIME_TO_DIE				2
#define ERROR_TIME_TO_EAT				3
#define ERROR_TIME_TO_SLEEP				4
#define ERROR_QUANTITY_PHILOSOPHER_EAT	5
#define ERROR_PHILOSOPH_DIE				6
#define ERROR_PHILOSOPHERS_ATE			7

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
	pthread_mutex_t *left;
	pthread_mutex_t *right;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number;
	int				count_eat;
	ssize_t			time_life;
	ssize_t			spawn_time;
	pthread_mutex_t *write_in_chat;
}					t_philosoph;

typedef struct			s_global
{
	t_arguments			*arguments;
	t_philosoph			*philosophers;
	pthread_mutex_t		*mutexs;
	ssize_t				start_programm_time;
	int					status_working;
	pthread_mutex_t 	*write_in_chat;
}						t_global;

ssize_t		get_current_time()
{
	struct timeval	time;
	ssize_t			time_start;

	gettimeofday(&time, NULL);
	time_start = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
	return (time_start);
}

void		my_usleep(long time) 
{
	struct timeval	t1;
	ssize_t			current_time;

	gettimeofday(&t1, NULL);
	current_time = get_current_time();
	while (time > (t1.tv_sec * 1000 + t1.tv_usec / 1000) - current_time)
	{
		gettimeofday(&t1, NULL);
		usleep(100);
	}
}


ssize_t		time_life_philosoph(ssize_t start_time) 
{
	struct timeval	time;
	ssize_t			time_start;

	gettimeofday(&time, NULL);
	time_start = ((time.tv_sec * 1000) + (time.tv_usec / 1000)) - start_time;
	return (time_start);
}


void	error(char *message) 
{
	printf("%s\n", message);
	exit(1);
}

// аргументы
// 1 - число философов от 1 до 200
// 2 - время смерти каждого философова
// 3 - время для еды
// 4 - время для сна
// 5 - сколько каждый философ должен есть

int		fill_struct(char **argv, t_arguments *arguments) 
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
	// number = ft_atoi(argv[5]); // сколько каждый философ должен есть
	// if (number < 1)
	// 	return (ERROR_QUANTITY_PHILOSOPHER_EAT);
	// arguments->nbr_of_times_each_philo_must_eat = number;
	return (WORK_IS_FINE);
}

// четные философы берут правыe, потом левые
// нечетные философы берут левые, потом правые

void	philosoph_eat(t_philosoph *philosoph) 
{
	pthread_mutex_lock(philosoph->write_in_chat);
	if ((philosoph->number % 2) == 0) // четные
	{
		pthread_mutex_lock(philosoph->right);
		printf("Take RIGHT fork %d!\n", philosoph->number);
		pthread_mutex_lock(philosoph->left);
		printf("Take LEFT fork %d!\n", philosoph->number);
		printf("Philosoph number: %d, eat!\n", philosoph->number);
	}
	else // нечетные
	{
		pthread_mutex_lock(philosoph->left);
		// pthread_mutex_lock(&philosoph->write_in_chat);
		printf("Take LEFT fork %d!\n", philosoph->number);
		printf("Philosoph number: %d, eat!\n", philosoph->number);
		pthread_mutex_lock(philosoph->right);
		printf("Take RIGHT fork %d!\n", philosoph->number);
	}
	pthread_mutex_unlock(philosoph->write_in_chat);
	my_usleep(philosoph->time_to_eat);
	philosoph->time_life = time_life_philosoph(0);//(philosoph->time_to_eat);
	pthread_mutex_unlock(philosoph->left);
	printf("Give LEFT fork %d!\n", philosoph->number);
	pthread_mutex_unlock(philosoph->right);
	printf("Give RIGHT fork %d!\n", philosoph->number);
}

void	philosoph_sleep(t_philosoph *philosoph) 
{
	pthread_mutex_lock(philosoph->write_in_chat);
	// printf("%d\n", philosoph->time_to_sleep);
	printf("Philosoph %d sleep\n", philosoph->number);
	pthread_mutex_unlock(philosoph->write_in_chat);
	my_usleep(philosoph->time_to_sleep);
	// philosoph->time_life += philosoph->time_to_sleep * 1000;
}

void	philosoph_thinking(t_philosoph *philosoph)
{
	pthread_mutex_lock(philosoph->write_in_chat);
	printf("Philosoph %d thinking\n", philosoph->number);
	// printf("%d\n", philosoph->time_to_sleep);
	pthread_mutex_unlock(philosoph->write_in_chat);
}

// void	philosoph_die(t_philosoph *philosoph) 
// {

// }

//							  Конкретный философ
void	*proccess_philosopher(void *philosopher)
{
	// printf("%d\n", (*(t_philosoph *)philosopher).time_to_sleep);
	struct timeval t1;

	gettimeofday(&t1, NULL);
	while (1)
	{
		(*(t_philosoph *)philosopher).time_life = time_life_philosoph((*(t_philosoph *)philosopher).spawn_time);
		// printf("%zd\n", (*(t_philosoph *)philosopher).time_life);
		philosoph_eat((t_philosoph *)philosopher);
		philosoph_sleep((t_philosoph *)philosopher);
		philosoph_thinking((t_philosoph *)philosopher);
		// if ((*(t_philosoph *)philosopher).time_life >= (*(t_philosoph *)philosopher).time_to_die)

	}
	return (NULL);
}

void	fill_settings_philosopher(t_global *global, t_philosoph *philosopher) 
{
	philosopher->time_to_die = global->arguments->time_to_die;
	philosopher->time_to_eat = global->arguments->time_to_eat;
	philosopher->time_to_sleep = global->arguments->time_to_sleep;
}

void	*watcher(void *global)
{
	int i;
	int time_to_die;
	int count_eat;
	int number_gorged_philosophers;

	i = 0;
	time_to_die = (*(t_global *)global).philosophers[i].time_to_die;
	count_eat = (*(t_global *)global).arguments->nbr_of_times_each_philo_must_eat;
	number_gorged_philosophers = 0;
	while (1)
	{
		while (i < (*(t_global *)global).arguments->quantity_philosophers)
		{
			if ((*(t_global *)global).philosophers[i].time_life > time_to_die)
			{
				(*(t_global *)global).status_working = ERROR_PHILOSOPH_DIE;
				pthread_mutex_lock((*(t_global *)global).write_in_chat);
				printf("Philosoph %d die\n", (*(t_global *)global).philosophers[i].number);
				return (NULL);
			}
			if (count_eat != -1)
			{
				if (number_gorged_philosophers == (*(t_global *)global).arguments->quantity_philosophers)
				{
					(*(t_global *)global).status_working = ERROR_PHILOSOPHERS_ATE;
					pthread_mutex_lock((*(t_global *)global).write_in_chat);
					printf("Philosoph %d die\n", (*(t_global *)global).philosophers[i].number);
					return (NULL);
				}
			}
			i++;
		}
		i = 0;
	}
	return (NULL);
}

int	philosopher_start(t_global *global)
{
	int i;
	// pthread_t threads[global->arguments->quantity_philosophers];
	pthread_t *threads = malloc(sizeof(pthread_t) * global->arguments->quantity_philosophers);
	pthread_t watching;
	pthread_mutex_t chat;

	i = 0;
	// создаю вилки на столе
	global->mutexs = malloc(sizeof(pthread_mutex_t) * global->arguments->quantity_philosophers);
	while (i < global->arguments->quantity_philosophers)
	{
		pthread_mutex_init(&global->mutexs[i], NULL);
		i++;
	}
	i = 0;
	// ресаю философов
	global->philosophers = malloc(sizeof(t_philosoph) * global->arguments->quantity_philosophers);
	// кидаю философам общий чат
	pthread_mutex_init(&chat, NULL);
	global->write_in_chat = &chat;
	while (i < global->arguments->quantity_philosophers)
	{
		global->philosophers[i].write_in_chat = &chat;
		i++;
	}
	i = 0;
	while (i < global->arguments->quantity_philosophers)
	{
		global->philosophers[i].number = i;
		global->philosophers[i].left = &global->mutexs[i];
		global->philosophers[i].right = &global->mutexs[(i + 1) % global->arguments->quantity_philosophers];
		global->philosophers[i].spawn_time = get_current_time();
		fill_settings_philosopher(global, &global->philosophers[i]);
		pthread_create(&threads[i], NULL, proccess_philosopher, (void *)(&global->philosophers[i]));
		i++;
	}
	pthread_create(&watching, NULL, watcher, (void *)global);
	i = 0;
	void *ptr;
	int error = pthread_join(watching, &ptr/*NULL*/);
	if (ptr != NULL)
	{
			// printf("Error!!!!\n");
		// exit(1);
		while (i < global->arguments->quantity_philosophers)
		{
			// pthread_join(threads[i], NULL);
			i++;
		}
	}
	// if (global->status_working == ERROR_PHILOSOPH_DIE || global->status_working == ERROR_PHILOSOPHERS_ATE)
	// 	return (1);
	// pthread_mutex_destroy(&global->write_in_chat);
}

int		main(int argc, char **argv)
{
	t_arguments arguments;
	t_global	global;
	int			check_status;

	// if (argc != 6 || argc != 5)
	// 	error("Error: arguments!");
	// else
	// {
		check_status = fill_struct(argv, &arguments);
		printf("%d %d %d %d %d\n", arguments.quantity_philosophers, 
		arguments.time_to_die, arguments.time_to_eat, arguments.time_to_sleep, 
		arguments.nbr_of_times_each_philo_must_eat);
		global.arguments = &arguments;
		if (check_status != 0)
			error("Error: arguments not fine!");
		else
		{
			global.start_programm_time = get_current_time();
			check_status = philosopher_start(&global);
		}
	// }
}