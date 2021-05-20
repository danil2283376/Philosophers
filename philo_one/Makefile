NAME = philo_one

SRC = philo_one.c philo_one.h additional_function.c additional_function1.c parsing_argument.c proccess_philosoph.c watcher_proccess.c ../ft_atoi.c

CC = gcc -Wall -Werror -Wextra

HEAD = philo_one.h philo_one_structs.h

all:$(NAME)

$(NAME):$(SRC) 
	$(CC) $(SRC)
	mv a.out ./philo_one

clean:
	rm -rf $(NAME)

fclean: clean
		rm *.gch

norm:
	norminette $(SRC)
	norminette 

re: fclean all

PHONY: all clean fclean re