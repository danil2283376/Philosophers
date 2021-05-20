#include "philo_one.h"

void	take_right_fork(t_philosoph *philosoph, int number)
{
	pthread_mutex_lock(philosoph->write_in_chat);
	printf("time %zu, Take RIGHT fork %d!\n", \
		get_time(philosoph->start_programm_time), number);
	pthread_mutex_unlock(philosoph->write_in_chat);
}
