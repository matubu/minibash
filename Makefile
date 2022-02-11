# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/15 15:41:05 by acoezard          #+#    #+#              #
#    Updated: 2021/11/25 15:25:05 by acoezard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	minishell

SOURCES	:=	./src
INCLUDES:=	./includes
OBJECTS	:=	./bin

SRCS	:=	minishell \
			print \
			char \
			redirection \
			heredocs \
			wildcard \
			orand \
			run/run \
			run/run_addons \
			lexer/lexer \
			lexer/lexer_addons \
			pipe/pipe \
			pipe/pipe_split \
			pipe/pipe_addons \
			builtin/builtin \
			builtin/builtin_env \
			token/token \
			token/token_addons \
			env/env \
			env/env_expand \
			str/mem \
			str/str \
			str/is \
			math

OBJS	:=	$(foreach src,$(SRCS),$(OBJECTS)/$(src).o)

FLAGS	:=	-Wall -Wextra -Werror -Iincludes
LINK	:=	libreadline.a -lreadline -lncurses

RED		:=	\033[31m
GRE		:=	\033[32m
GRA		:=	\033[37m
BLU		:=	\033[34m
EOC		:=	\033[0m

${OBJECTS}/%.o: ${SOURCES}/%.c
	@echo "$(BLU)● Compiling $^ 🔧$(EOC)"
	@mkdir -p $(dir $@)
	@gcc $(FLAGS) $^ -c -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GRE)● Compiling $(NAME) ⚙️ $(EOC)"
	@gcc $(FLAGS) $(OBJS) $(LINK) -o $(NAME)
clean:
	@echo "$(RED)● Removing objects 📁$(EOC)"
	@rm -rf ${OBJECTS}

fclean: clean
	@echo "$(RED)● Removing binary ⚙️ $(EOC)"
	@rm -rf $(NAME)

re: fclean all

run: all
	./$(NAME)

bonus: $(NAME)

.PHONY: all clean fclean re run bonus
