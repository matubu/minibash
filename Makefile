# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/15 15:41:05 by acoezard          #+#    #+#              #
#    Updated: 2021/11/23 11:45:25 by mberger-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

SRC		=	minishell \
			print \
			pipe \
			pipe_split \
			char \
			redirection \
			heredocs \
			string_1 \
			string_2 \
			run \
			builtin \
			builtin_env \
			lexer \
			lexer_addons \
			token \
			env \
			env_expand \
			wildcard \
			orand

OBJ		=	$(foreach src,$(SRC),bin/$(src).o)

FLAGS	=	-Wall -Wextra -Werror -Iincludes
LINK	=	libreadline.a -lreadline -lncurses# -g# -fsanitize=address

RED		=	\033[31m
GRE		=	\033[32m
GRA		=	\033[37m
BLU		=	\033[34m
EOC		=	\033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@echo "⚙️  $(GRE)Compilation de ${NAME}...$(EOC)"
	@gcc $(OBJ) $(LINK) -o $(NAME)

bin/%.o: src/%.c
	@echo "🔧 Compilation de $(BLU)${notdir $<}$(EOC)."
	@mkdir -p bin
	@gcc $(FLAGS) $^ -c -o $@

clean:
	@echo "$(RED)📁 Supression des fichiers binaires (.o)...$(EOC)"
	@rm -rf bin

fclean: clean
	@echo "$(RED)⚙️  Supression des executables et librairies...$(EOC)"
	@rm -rf $(NAME)

re: fclean all

run: all
	./$(NAME)

bonus: $(NAME)

.PHONY: all clean fclean re run bonus
