/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:28 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/15 16:33:40 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************* INCLUDES ************************* */
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <sys/wait.h>

/* ************************* DEFINES ************************** */
# define NAME		"minishell"
# define PS1		"\033[32mminishell$\033[0m "
# define PATH_BUF	256

/* ************************ STRUCTURES ************************ */
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

/* ********************** INPUT / OUTPUT ********************** */
void			putstr(int fd, char *s);
void			println(int fd, char *s);
char			*itoa_buf(int n);
int				error(char *name, char *err, char *info);
int				err(char *err, char *info);
void			rl_replace_line (const char *text, int clear_undo);

/* ************************** STRINGS ************************* */
int				ft_strlen(const char *s);
unsigned int	ft_strlcpy(char *dst, const char *src, unsigned int size);
int				ft_strisonly(const char *s, char c);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strcat(char *dest, const char *src);
char			*ft_strdup(char *str);
char			*ft_strchr(const char *s, int c);

/* *************************** LEXER ************************** */
t_token			*create_tokens(char *s);
int				free_tokens(t_token *tokens);
char			**token_to_argv(t_token *tokens);

/* ******************** ENVIRONEMENT LOCALS ******************* */
int				ispartofenv(char c);
int				isenvdefine(char *s);
void			env_expand(char **env, t_token *tokens);
char			**env_get(char **env, char *key);
void			env_set(char ***env, char *kv);

/* ************************* WILDCARDS ************************ */
void			wildcard_expand(t_token *tokens);

/* ************************* RUNTIME ************************** */
char			*pathncat(char *path, int n, char *relative);
void			run(char *cmd, char **argv, t_env *env);

/* ************************ BUILT-INS ************************* */
void			cd_builtin(char **argv);
void			echo_builtin(char **argv);
void			pwd_builtin(char **argv);
void			env_builtin(char **env);
void			exit_builtin(char **argv);
void			unset_builtin(char **argv, t_env *env);
void			export_builtin(char **argv, t_env *env);
void			set_builtin(char **argv, t_env *env);

#endif