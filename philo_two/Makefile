NAME = philo_two

SRC = philo_two.c philo_two.h additional_function.c parsing_arguments.c philosopher_proccess.c watcher_proccess.c ../ft_atoi.c

CC = gcc -Wall -Werror -Wextra

HEAD = philo_two.h philo_two_structs.h

all:$(NAME)

$(NAME):$(SRC) 
	$(CC) $(SRC)
	mv a.out ./philo_two

clean:
	rm -rf $(NAME)

fclean: clean
		rm *.gch

norm:
	norminette $(SRC)
	norminette 

re: fclean all

PHONY: all clean fclean re