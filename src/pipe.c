/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/24 17:06:51 by acoezard         ###   ########.fr       */
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
	int	i;
	int	fd[2];

	if (stdin)
		(void)dup2(stdin, 0);
	if (s)
	{
		pipe(fd);
		i = -1;
		while (s[++i])
			write(fd[1], s + i, 1);
		close(fd[1]);
	}
	while (redirs->value)
	{
		if (*redirs->value && redirs->type == REDIR_LEFT)
		{
			redirs->fd = open(redirs->value + 1, O_RDONLY);
			if (redirs->fd == -1)
			{
				err(redirs->value + 1, "Permission denied");
				break ;
			}
			dup2(redirs->fd, 0);
		}
		else if (s && *redirs->value && redirs->type == REDIR_HD_LEFT)
			dup2(fd[0], 0);
		redirs++;
	}
}

void	pipe_execute(t_env *env, char **subcmds, int stdin)
{
	t_redirection	*redirs;
	int				fd[2];
	pid_t			pid;
	int				builtin;
	char			*s;

	redirs = exec_redirections(*subcmds, env);
	if (redirs == NULL)
		return ;
	if (!exec_heredocs(redirs + 1, &s))
	{
		pipe(fd);
		builtin = 1;
		pid = exec_builtin(redirs->value, env, get_fd(subcmds[1], fd[1]));
		if (!pid && builtin--)
			pid = fork();
		if (pid == -1)
			return ((void)err("fork", "resource temporarily unavailable"));
		if (pid == 0)
		{
			redirect_in(stdin, redirs + 1, s);
			if (subcmds[1])
				dup2(fd[1], 1);
			close(fd[0]);
			if (redirect_out(redirs + 1) || exec_tokens(redirs->value, env))
			{
				close_out(redirs + 1);
				exit(127);
			}
			close_out(redirs + 1);
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
		if (s)
			free(s);
	}
	free_redirections(redirs);
}
