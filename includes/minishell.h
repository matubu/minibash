/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:28 by mberger-          #+#    #+#             */
/*   Updated: 2021/12/02 12:38:24 by mberger-         ###   ########.fr       */
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
# include <dirent.h>
# include <fcntl.h>

/* ************************* DEFINES ************************** */
# define NAME			"minishell"
# define PS1			"minishell$ "
# define PATH_BUF		4096

# define REDIR_NOT		0b0000
# define REDIR_LEFT		0b0001
# define REDIR_RIGHT	0b0010
# define REDIR_HD_LEFT	0b0100
# define REDIR_HD_RIGHT	0b1000

/* ************************ STRUCTURES ************************ */
typedef struct s_token
{
	char	*value;
	int		expendable;
}	t_token;

typedef struct s_process
{
	int		pid;
	int		code;
	char	*pwd;
}	t_process;

extern t_process	g_process;

typedef struct s_env
{
	char	**local;
	char	**exported;
}	t_env;

typedef struct s_redirection
{
	char	*value;
	int		type;
	int		expanded;
	int		fd;
}	t_redirection;

/* ********************** INPUT / OUTPUT ********************** */
void			putstr(int fd, char *s);
void			println(int fd, char *s);
char			*itoa_buf(int n);
int				error(char *name, char *err, char *info, int code);
int				err(char *err, char *info, int code);
void			rl_replace_line(const char *text, int clear_undo);

/* ************************** STRINGS ************************* */
int				ft_strlen(const char *s);
unsigned int	ft_strlcpy(char *dst, const char *src, unsigned int size);
int				ft_strisonly(const char *s, char *charset);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strcat(char *dest, const char *src);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strdup(char *str);
char			*ft_strchr(const char *s, int c);
char			*ft_substr(char *s, int n);

char			*char_cat(char *dest, const char src);
char			*char_join(const char *s1, const char c);

int				is_space(char c);
int				is_operator(char c);
int				ft_atol(const char *nptr, long *v);

/* *************************** PIPES ************************** */
void			pipe_parse(t_env *env, char *cmd);
char			**pipe_split(char *s);
void			pipe_execute(t_env *env, char **subcmds, int stdin);
void			close_out(t_redirection *redirs);

/* *************************** LEXER ************************** */
int				tokenize(char *s, int (*token)(), void *arg);
int				is_operator(char c);
int				is_space(char c);
t_token			**create_tokens(char *s);
int				free_tokens(t_token **tokens);
char			**token_to_argv(t_token **tokens);
char			**create_argv(char *cmd, t_env *env);
char			*pathncat(char *path, int n, char *relative);
int				runfrompath(char *cmd, char **argv, char **env);
int				runsearch(char *cmd, char **argv, char **env);
int				free_argv(char **argv);

/* ******************** ENVIRONEMENT LOCALS ******************* */
int				ispartofenv(char c);
int				isenvdefine(char *s);
void			env_init(t_env *env, char **envm);
void			env_expand(char **env, t_token **tokens);
char			**env_get(char **env, char *key);
void			env_set(char ***env, char *kv);
char			*env_replace(char **env, char *s);

/* ************************* WILDCARDS ************************ */
void			wildcard_expand(t_token ***tokens);

/* *********** REDIRECTIONS / HEREDOCS / OPERATIONS *********** */
t_redirection	*exec_redirections(char *cmd, t_env *env);
int				exec_heredocs(t_redirection *redirs, char **buffer);
int				free_redirections(t_redirection *redir);
int				redirect_out(t_redirection *redirs);
void			redirect_in(int stdin, t_redirection *redirs, char *s);
void			unredirect_out(t_redirection *redirs);
int				get_flag(int type);

/* ************************ || && () ************************** */
char			*orand(t_env *env, char *s, int exec, int brace);

/* ************************* RUNTIME ************************** */
int				exec_builtin(char *cmd, t_env *env, int stdout);
int				exec_tokens(char *cmd, t_env *env);

/* ************************ BUILT-INS ************************* */
void			cd_builtin(t_env *env, char **argv);
void			echo_builtin(int stdout, char **argv);
void			pwd_builtin(int stdout);
void			env_builtin(int stdout, char **env);
void			exit_builtin(int stdout, char **argv);
void			unset_builtin(char **argv, t_env *env);
void			export_builtin(int stdout, char **argv, t_env *env);
void			set_builtin(char **argv, t_env *env);

#endif
