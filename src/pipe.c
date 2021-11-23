/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/23 20:29:25 by matubu           ###   ########.fr       */
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
		return ((void)dup2(stdin, 0));
	if (s)
	{
		pipe(fd);
		i = -1;
		while (s[++i])
			write(fd[1], s + i, 1);
		close(fd[1]);
		free(s);
	}
	while (redirs->value)
	{
		if (*redirs->value && redirs->type == REDIR_LEFT)
		{
			redirs->fd = open(redirs->value + 1, O_RDONLY);
			if (redirs->fd == -1)
			{
				err(redirs->value + 1, "Permission denied");
				break;
			}
			dup2(redirs->fd, 0);
		}
		else if (s && *redirs->value && redirs->type == REDIR_HD_LEFT)
			dup2(fd[0], 0);
		redirs++;
	}
}

void	unredirect_out(t_redirection *redirs)
{
	dup2(redirs->old, 1);
	while (redirs->value)
	{
		if ((redirs->type == REDIR_RIGHT || redirs->type == REDIR_HD_RIGHT
				|| redirs->type == REDIR_LEFT) && redirs->fd)
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
	char			*s;

	printf("exec %s\n", *subcmds);
	redirs = exec_redirections(*subcmds, env);
	if (redirs == NULL)
		return ;
	printf("realexec %s\n", redirs->value);
	if (!redirect_out(redirs + 1) && !exec_heredocs(redirs, &s))
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
	}
	unredirect_out(redirs + 1);
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
