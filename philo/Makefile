NAME = philo

CC = gcc

FLAGS = -Wall -Wextra -Werror

SRC = ./philosophers.c\
	./ft_parsing.c\
	./ft_actions.c\
	./ft_init.c\
	./ft_check_exit.c\
	./ft_utils.c\
	./ft_time.c

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@ -l pthread

%.o : %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean all fclean re
