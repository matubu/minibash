/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:28 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/12 12:10:46 by mberger-         ###   ########.fr       */
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

typedef struct s_process
{
	int	pid;
	int	code;
}	t_process;
extern t_process	g_process;

typedef struct s_env
{
	char	**local;
	char	**exported;
}	t_env;

//str
int				ft_strlen(const char *s);
unsigned int	ft_strlcpy(char *dst, const char *src, unsigned int size);
int				ft_strisonly(const char *s, char c);
int				ft_strcmp(const char *s1, const char *s2);
//TODO remove strcat
char			*ft_strcat(char *dest, const char *src);
char			*ft_strdup(char *str);
char			*ft_strchr(const char *s, int c);

//token + lexer
t_token			*create_tokens(char *s);
int				free_tokens(t_token *tokens);
char			**token_to_argv(t_token *tokens);

//env + expander
int				ispartofenv(char c);
int				isenvdefine(char *s);
void			env_expend(char **env, t_token *tokens);
char			**env_get(char **env, char *key);
void			env_set(char ***env, char *kv);

//print
void			putstr(int fd, char *s);
void			println(int fd, char *s);
char			*itoa_buf(int n);
int				error(char *name, char *err, char *info);
int				err(char *err, char *info);

//run
char			*pathncat(char *path, int n, char *relative);
void			run(char *cmd, char **argv, t_env *env);

//buildin
void			cd_buildin(char **argv);
void			echo_buildin(char **argv);
void			pwd_buildin(char **argv);
void			env_buildin(char **env);
void			exit_buildin(char **argv);
void			unset_buildin(char **argv, t_env *env);
void			export_buildin(char **argv, t_env *env);
void			set_buildin(char **argv, t_env *env);

//readline
void			rl_replace_line (const char *text, int clear_undo);

#endif
