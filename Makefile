NAME = minishell
OBJ = run.o print.o
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(OBJ) -o $(NAME)

%.o: %.c
	gcc $(FLAGS) $^ -c -o $@

clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(NAME)

re: fclean all

bonus: $(NAME)

.PHONY: all clean fclean re bonus
