/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:28 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/11 18:37:17 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//stat
# include <sys/stat.h>
//read write
# include <unistd.h>
//malloc free
# include <stdlib.h>
//strerror
# include <string.h>
//errno
# include <errno.h>
//readline
# include <readline/readline.h>
# include <readline/history.h>
//kill
#include <signal.h>
//tcgetattr tcsetattr (hide ^C and ^\)
#include <termios.h>
//wait
#include <sys/wait.h>

# define NAME "minishell"
# define PS1 "\033[32mminishell$\033[0m "
# define PATH_BUF 256

typedef struct s_token
{
	char	*value;
	int		expendable;
}	t_token;

int				ft_strlen(const char *s);
unsigned int	ft_strlcpy(char *dst, const char *src, unsigned int size);
int				ft_strcmp(const char *s1, const char *s2);

t_token			*create_tokens(char *s);
int				free_tokens(t_token *tokens);
char			**token_to_argv(t_token *tokens);

void			putstr(int fd, char *s);
void			println(int fd, char *s);
void			putint(int fd, int n);
int				error(char *name, char *err, char *info);
int				err(char *err, char *info);

char			*pathncat(char *path, int n, char *relative);
void			run(char *cmd, char **argv, char **env);

//buildin
void			cd(char **argv);
void			echo(char **argv);
void			pwd(char **argv);

//readline
void			rl_replace_line (const char *text, int clear_undo);

extern int	g_process;

#endif
