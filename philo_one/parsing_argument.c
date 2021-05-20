#include "philo_one.h"

int	threatment_six_arguments(int number, char **argv, t_arguments *arguments)
{
	number = ft_atoi(argv[5]);
	if (number < 1)
		return (ERROR_QUANTITY_PHILOSOPHER_EAT);
	arguments->nbr_of_times_each_philo_must_eat = number;
	return (WORK_IS_FINE);
}

int	fill_struct(int argc, char **argv, t_arguments *arguments)
{
	int	number;

	number = ft_atoi(argv[1]);
	if (number < 1 || number > 200)
		return (ERROR_QUANTITY_PHILOSOPHERS);
	arguments->quantity_philosophers = number;
	number = ft_atoi(argv[2]);
	if (number < 1)
		return (ERROR_TIME_TO_DIE);
	arguments->time_to_die = number;
	number = ft_atoi(argv[3]);
	if (number < 1)
		return (ERROR_TIME_TO_EAT);
	arguments->time_to_eat = number;
	number = ft_atoi(argv[4]);
	if (number < 1)
		return (ERROR_TIME_TO_SLEEP);
	arguments->time_to_sleep = number;
	if (argc == 6)
	{
		return (threatment_six_arguments(number, argv, arguments));
	}
	return (WORK_IS_FINE);
}
