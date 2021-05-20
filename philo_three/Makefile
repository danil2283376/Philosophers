NAME = philo_three

SRC = philo_three.c philo_three.h additional_function.c parsing_arguments.c proccess_philosoph.c watcher_proccess.c ../ft_atoi.c

CC = gcc -Wall -Werror -Wextra

HEAD = philo_three.h philo_three_structs.h

all:$(NAME)

$(NAME):$(SRC) 
	$(CC) $(SRC)
	mv a.out ./philo_three

clean:
	rm -rf $(NAME)

fclean: clean
		rm *.gch

norm:
	norminette $(SRC)
	norminette 

re: fclean all

PHONY: all clean fclean re