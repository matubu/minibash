NAME = minishell
SRC = print str run main buildin buildin_env lexer token env expander
OBJ = $(foreach src,$(SRC),bin/$(src).o)

FLAGS = -Wall -Wextra -Werror -Iincludes
LINK = libreadline.a -lreadline -lncurses -fsanitize=address

RED = \033[31m
GRE = \033[32m
GRA = \033[37m
BLU = \033[34m
EOC = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(GRE)● Compiling $(NAME) ⚙️ $(EOC)"
	@gcc $(OBJ) $(LINK) -o $(NAME)

bin/%.o: src/%.c
	@echo "$(BLU)● Compiling $^ 🔧$(EOC)"
	@mkdir -p bin
	@gcc $(FLAGS) $^ -c -o $@

clean:
	@echo "$(RED)● Removing objects 📁$(EOC)"
	@rm -rf bin

fclean: clean
	@echo "$(RED)● Removing binary ⚙️ $(EOC)"
	@rm -rf $(NAME)

re: fclean all

run: all
	./$(NAME)

bonus: $(NAME)

.PHONY: all clean fclean re run bonus
