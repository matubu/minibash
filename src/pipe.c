/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/23 11:01:48 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_fd(char *subcmds, int fd)
{
	if (subcmds)
		return (fd);
	return (1);
}

static int	get_flag(int type)
{
	if (type == REDIR_HD_RIGHT)
		return (O_CREAT | O_WRONLY | O_APPEND);
	return (O_CREAT | O_WRONLY | O_TRUNC);
}

void	redirect_out(t_redirection *redirs)
{
	redirs->old = dup(1);
	while (redirs->value)
	{
		if (*redirs->value && (redirs->type == REDIR_RIGHT
				|| redirs->type == REDIR_HD_RIGHT))
		{
			redirs->fd
				= open(redirs->value + 1, get_flag(redirs->type), S_IRWXU);
			dup2(redirs->fd, 1);
		}
		redirs++;
	}
}

void	close_all(t_redirection *redirs)
{
	dup2(redirs->old, 1);
	while (redirs->value)
	{
		if (redirs->type == REDIR_RIGHT || redirs->type == REDIR_HD_RIGHT)
			close(redirs->fd);
		redirs++;
	}
}

static void	pipe_execute(t_env *env, char **subcmds, int stdin)
{
	t_redirection	*redirs;
	int				fd[2];
	pid_t			pid;
	int				builtin;

	redirs = exec_redirections(*subcmds, env);
	exec_heredocs(redirs);
	pipe(fd);
	redirect_out(redirs + 1);
	builtin = 1;
	pid = exec_builtin(redirs->value, env, get_fd(subcmds[1], fd[1]));
	if (!pid && builtin--)
		pid = fork();
	if (pid == 0)
	{
		dup2(stdin, 0);
		if (subcmds[1])
			dup2(fd[1], 1);
		close(fd[1]);
		if (exec_tokens(redirs->value, env))
			exit(127);
		exit(0);
	}
	close(fd[1]);
	if (subcmds[1])
		pipe_execute(env, subcmds + 1, fd[0]);
	close(fd[0]);
	g_process.pid = pid;
	if (subcmds[1])
		kill(pid, SIGINT);
	if (!builtin && ++builtin)
	{
		waitpid(pid, &g_process.code, 0);
		g_process.code = WEXITSTATUS(g_process.code);
	}
	close_all(redirs + 1);
	free_redirections(redirs);
}

void	pipe_parse(t_env *env, char *cmd)
{
	char	**subcmds;
	int		i;

	subcmds = pipe_split(cmd);
	if (subcmds == NULL)
		return ((void)free(cmd));
	i = 0;
	while (subcmds[i] && subcmds[i + 1])
	{
		if (*subcmds[++i] == '\0')
		{
			free_argv(subcmds);
			free(cmd);
			return ((void)err("syntax error near unexpected token", "|"));
		}
	}
	if (*subcmds && **subcmds)
		pipe_execute(env, subcmds, 0);
	free_argv(subcmds);
	free(cmd);
}
