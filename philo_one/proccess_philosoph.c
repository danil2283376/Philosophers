#include "philo_one.h"

void	take_fork(t_philosoph *philosoph, int number)
{
	if ((number % 2) == 0)
	{
		pthread_mutex_lock(philosoph->right);
		pthread_mutex_lock(philosoph->write_in_chat);
		printf("time %zu, Take RIGHT fork %d!\n", \
			get_time(philosoph->start_programm_time), number);
		pthread_mutex_unlock(philosoph->write_in_chat);
		pthread_mutex_lock(philosoph->left);
		pthread_mutex_lock(philosoph->write_in_chat);
		printf("time %zu, Take LEFT fork %d!\n", \
			get_time(philosoph->start_programm_time), number);
		pthread_mutex_unlock(philosoph->write_in_chat);
	}
	else
	{
		pthread_mutex_lock(philosoph->left);
		pthread_mutex_lock(philosoph->write_in_chat);
		printf("time %zu, Take LEFT fork %d!\n", \
			get_time(philosoph->start_programm_time), number);
		pthread_mutex_unlock(philosoph->write_in_chat);
		pthread_mutex_lock(philosoph->right);
		take_right_fork(philosoph, number);
	}
}

void	philosoph_eating(t_philosoph *philosoph)
{
	if ((philosoph->number % 2) == 0)
	{
		pthread_mutex_lock(philosoph->write_in_chat);
		printf("time %zu, Philosoph number: %d, eat!\n", \
			get_time(philosoph->start_programm_time), philosoph->number);
		++philosoph->count_eat;
		philosoph->time_life = get_time(0);
		pthread_mutex_unlock(philosoph->write_in_chat);
		my_usleep(philosoph->time_to_eat);
		pthread_mutex_unlock(philosoph->right);
		pthread_mutex_unlock(philosoph->left);
	}
	else
	{
		pthread_mutex_lock(philosoph->write_in_chat);
		printf("time %zu, Philosoph number: %d, eat!\n", \
			get_time(philosoph->start_programm_time), philosoph->number);
		++philosoph->count_eat;
		philosoph->time_life = get_time(0);
		pthread_mutex_unlock(philosoph->write_in_chat);
		my_usleep(philosoph->time_to_eat);
		pthread_mutex_unlock(philosoph->left);
		pthread_mutex_unlock(philosoph->right);
	}
}

void	philosoph_sleeping(t_philosoph *philosoph)
{
	pthread_mutex_lock(philosoph->write_in_chat);
	printf("time %zu, Philosoph number: %d, sleeping!\n", \
		get_time(philosoph->start_programm_time), philosoph->number);
	pthread_mutex_unlock(philosoph->write_in_chat);
	my_usleep(philosoph->time_to_sleep);
}

void	philosoph_thinking(t_philosoph *philosoph)
{
	pthread_mutex_lock(philosoph->write_in_chat);
	printf("time %zu, Philosoph number: %d, thinking!\n", \
		get_time(philosoph->start_programm_time), philosoph->number);
	pthread_mutex_unlock(philosoph->write_in_chat);
}

void	*proccess_philosoph(void *philosoph)
{
	pthread_detach((*(t_philosoph *)philosoph).name_flow);
	(*(t_philosoph *)philosoph).time_life = get_time(0);
	while (1)
	{
		take_fork((t_philosoph *)philosoph, (*(t_philosoph *)philosoph).number);
		philosoph_eating((t_philosoph *)philosoph);
		philosoph_sleeping((t_philosoph *)philosoph);
		philosoph_thinking((t_philosoph *)philosoph);
	}
}
