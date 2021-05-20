#include "philo_two.h"

void	philosoph_eat(t_philosoph *philosoph, int number)
{
	sem_wait(philosoph->waiter);
	sem_wait(philosoph->semaphors);
	sem_wait(philosoph->write_in_chat);
	printf("time %zu, Take RIGHT fork %d!\n", \
		get_time(philosoph->start_programm_time), number);
	sem_post(philosoph->write_in_chat);
	sem_wait(philosoph->semaphors);
	sem_wait(philosoph->write_in_chat);
	printf("time %zu, Take LEFT fork %d!\n", \
		get_time(philosoph->start_programm_time), number);
	sem_post(philosoph->waiter);
	sem_post(philosoph->write_in_chat);
	sem_wait(philosoph->write_in_chat);
	printf("time %zu, Philosoph number: %d, eat!\n", \
		get_time(philosoph->start_programm_time), number);
	++philosoph->count_eat;
	philosoph->time_life = get_time(0);
	sem_post(philosoph->write_in_chat);
	my_usleep(philosoph->time_to_eat);
	sem_post(philosoph->semaphors);
	sem_post(philosoph->semaphors);
}

void	philosoph_sleep(t_philosoph *philosoph)
{
	sem_wait(philosoph->write_in_chat);
	printf("time %zu, Philosoph number %d sleeping!\n", \
		get_time(philosoph->start_programm_time), philosoph->number);
	sem_post(philosoph->write_in_chat);
	my_usleep(philosoph->time_to_sleep);
}

void	philosoph_thinking(t_philosoph *philosoph)
{
	sem_wait(philosoph->write_in_chat);
	printf("time %zu, Philosoph number %d thinking!\n", \
		get_time(philosoph->start_programm_time), philosoph->number);
	sem_post(philosoph->write_in_chat);
}

void	*proccess_philosoph(void *philosoph)
{
	t_philosoph	*copy_philosoph;

	pthread_detach((*(t_philosoph *)philosoph).name_thread);
	copy_philosoph = (t_philosoph *)philosoph;
	sem_wait(copy_philosoph->security);
	copy_philosoph->time_life = get_time(0);
	sem_post(copy_philosoph->security);
	while (1)
	{
		philosoph_eat(copy_philosoph, copy_philosoph->number);
		philosoph_sleep(copy_philosoph);
		philosoph_thinking(copy_philosoph);
	}
}
