/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/18 18:30:47 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_execute(t_env	*env, char **subcmds, int stdin)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(stdin, 0);
		if (subcmds[1])
			dup2(fd[1], 1);
		close(fd[1]);
		exec_tokens(*subcmds, env);
		exit(0);
	}
	close(fd[1]);
	if (subcmds[1] && *subcmds[1] == '\0')
		return ((void)err("syntax error near unexpected token", "|"));
	if (subcmds[1])
		pipe_execute(env, subcmds + 1, fd[0]);
	close(fd[0]);
	g_process.pid = pid;
	if (subcmds[1])
		kill(pid, SIGINT);
	wait(&g_process.code);
}

// TODO: #30 exit cd
void	pipe_parse(t_env *env, char *cmd)
{
	char	**subcmds;

	subcmds = pipe_split(cmd);
	if (subcmds == NULL)
		return ;
	if (*subcmds)
		pipe_execute(env, subcmds, 0);
	free_argv(subcmds);
}
