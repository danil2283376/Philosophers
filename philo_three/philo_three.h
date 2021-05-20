#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <semaphore.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <signal.h>
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
	int				quantity_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_times_each_philo_must_eat;
}					t_arguments;

typedef struct s_philosoph
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				count_eat;
	int				number;
	ssize_t			time_life;
	sem_t			*write_in_chat;
	ssize_t			start_programm_time;
	sem_t			*semaphors;
	int				quantity_philo_start;
	int				quantity_philosoph;
	int				nbr_of_times_each_philo_must_eat;
	int				five_argument_exist;
	sem_t			*ones_die;
	sem_t			*defender;
	pid_t			*other_philosoph;
	int				count_philosophs;
}					t_philosoph;

typedef struct s_global
{
	t_arguments			*arguments;
	sem_t				*semaphors;
	ssize_t				start_programm_time;
	int					status_working;
	sem_t				*write_in_chat;
	int					optional_varible;
	pid_t				*philosoph_pid;
	sem_t				*ones_die;
	sem_t				*defender;
}						t_global;

void	my_usleep(ssize_t time);
ssize_t	get_time(ssize_t begin_time);
int		error(char *message);
int		fill_struct(int argc, char **argv, t_arguments *arguments);
void	philosoph_eat(t_philosoph *philosoph, int number);
void	philosoph_sleep(t_philosoph *philosoph, int number);
void	philosoph_thinking(t_philosoph *philosoph, int number);
void	*start_philosoph(void *philosoph);
void	*check_life(void *philosoph);
void	*check_count_eat(void *philosoph);
int		ft_atoi(const char *str);
#endif