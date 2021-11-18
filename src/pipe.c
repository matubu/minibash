/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/18 12:45:04 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_execute(t_env	*env, char **subcmds, int stdin)
{
	t_token	**tokens;
	int		fd[2];
	char	buf;
	pid_t	pid;

	if (*subcmds == NULL)
	{
		dup2(fd[0], 1);
//		close(fd[0]);
		while (read(stdin, &buf, 1))
			write(1, &buf, 1);
		//dup2(1, stdin);
		return ;
	}

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(stdin, 0);
		dup2(fd[1], 1);
		close(fd[1]);
		tokens = create_tokens(*subcmds);
		env_expand(env->local, tokens);
		wildcard_expand(&tokens);
		if (tokens[0]->value)
			exec_tokens(tokens, env);
		else
			g_process.code = 1;
		free_tokens(tokens);
		exit(-1);
	}
	else
	{
		close(fd[1]);
//		if (subcmds[1])
			pipe_execute(env, subcmds + 1, fd[0]);
		close(fd[0]);
		g_process.pid = pid;
		wait(&g_process.code);
	}
}

//TODO fix infinite loop with recursion ex: cat /dev/urandom | head -n 2
void	pipe_parse(t_env *env, char *cmd)
{
	char	**subcmds;

	subcmds = pipe_split(cmd);
	if (subcmds && *subcmds)
		pipe_execute(env, subcmds, 0);
	free(subcmds);
}
