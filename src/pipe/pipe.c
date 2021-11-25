/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/25 13:54:30 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_fd(char *subcmds, int fd)
{
	if (subcmds)
		return (fd);
	return (1);
}

void	redirect_in(int stdin, t_redirection *redirs, char *s)
{
	int	fd[2];

	if (stdin)
		(void)dup2(stdin, 0);
	if (s)
	{
		pipe(fd);
		putstr(fd[1], s);
		close(fd[1]);
	}
	while ((++redirs)->value)
	{
		if (*redirs->value && redirs->type == REDIR_LEFT)
		{
			redirs->fd = open(redirs->value + 1, O_RDONLY);
			if (redirs->fd == -1)
			{
				err(redirs->value + 1, "Permission denied", 1);
				break ;
			}
			dup2(redirs->fd, 0);
		}
		else if (s && *redirs->value && redirs->type == REDIR_HD_LEFT)
			dup2(fd[0], 0);
	}
}

static void	pipe_wait(char **subcmds, pid_t pid, char *s,
		t_redirection *redirs)
{
	if (pid)
	{
		g_process.pid = pid;
		if (subcmds[1])
			kill(pid, SIGINT);
		waitpid(pid, &g_process.code, 0);
		g_process.code = WEXITSTATUS(g_process.code);
	}
	if (s)
		free(s);
	free_redirections(redirs);
}

typedef struct s_arg
{
	t_env	*env;
	char	**subcmds;
	int		stdin;
}	t_arg;

static int	pipe_(t_arg arg, t_redirection *redirs, int *fd, char *s)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		redirect_in(arg.stdin, redirs, s);
		if (arg.subcmds[1])
			dup2(fd[1], 1);
		close(fd[0]);
		if (redirect_out(redirs + 1) || exec_tokens(redirs->value, arg.env))
		{
			close_out(redirs + 1);
			exit(127);
		}
		close_out(redirs + 1);
		exit(0);
	}
	return (pid);
}

void	pipe_execute(t_env *env, char **subcmds, int stdin)
{
	t_redirection	*redirs;
	int				fd[2];
	pid_t			pid;
	char			*s;

	redirs = exec_redirections(*subcmds, env);
	if (redirs == NULL)
		return ;
	if (exec_heredocs(redirs + 1, &s))
		return ((void)free_redirections(redirs));
	pipe(fd);
	pid = 0;
	if (!exec_builtin(redirs->value, env, get_fd(subcmds[1], fd[1])))
		pid = pipe_((t_arg){env, subcmds, stdin}, redirs, fd, s);
	close(fd[1]);
	if (subcmds[1])
		pipe_execute(env, subcmds + 1, fd[0]);
	close(fd[0]);
	pipe_wait(subcmds, pid, s, redirs);
}
