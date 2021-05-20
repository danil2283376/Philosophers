#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <semaphore.h>
# include <stdlib.h>

# define WORK_IS_FINE 0
# define ERROR_QUANTITY_PHILOSOPHERS 1
# define ERROR_TIME_TO_DIE 2
# define ERROR_TIME_TO_EAT 3
# define ERROR_TIME_TO_SLEEP 4
# define ERROR_QUANTITY_PHILOSOPHER_EAT 5
# define ERROR_PHILOSOPH_DIE 6
# define ERROR_PHILOSOPHERS_ATE 7
# define ERROR_CREATE_SIMOPHORE 8

typedef struct s_arguments
{
	int					quantity_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nbr_of_times_each_philo_must_eat;
}						t_arguments;

typedef struct s_philosoph
{
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number;
	int					count_eat;
	ssize_t				time_life;
	sem_t				*write_in_chat;
	ssize_t				start_programm_time;
	pthread_t			name_thread;
	sem_t				*semaphors;
	sem_t				*waiter;
	sem_t				*security;
	int					quantity_philo_start;
	int					quantity_philosoph;
}						t_philosoph;

typedef struct s_global
{
	t_arguments			*arguments;
	t_philosoph			*philosophers;
	sem_t				*semaphors;
	ssize_t				start_programm_time;
	int					status_working;
	sem_t				*write_in_chat;
	int					optional_varible;
	sem_t				*waiter;
	sem_t				*security;
}						t_global;

void	my_usleep(ssize_t time);
ssize_t	get_time(ssize_t begin_time);
int		error(char *message);
int		fill_struct(int argc, char **argv, t_arguments *arguments);
void	philosoph_eat(t_philosoph *philosoph, int number);
void	philosoph_sleep(t_philosoph *philosoph);
void	philosoph_thinking(t_philosoph *philosoph);
void	*proccess_philosoph(void *philosoph);
int		ft_atoi(const char *str);
void	*watcher_proccess(void *global);

#endif