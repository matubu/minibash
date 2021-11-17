/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/17 17:52:30 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_write(int tube[2], char *buffer, size_t size)
{
	write(tube[1], buffer, size);
    close(tube[0]);
    close(tube[1]);
}

static char	*pipe_read(int tube[2])
{
	char	*message;
	char	*tmp;
	char	buffer;

	message = ft_strdup("");
	close(tube[1]);
	while (1)
	{
		if (read(tube[0], &buffer, 1) < 1)
			break;
		tmp = char_join(message, buffer);
		free(message);
		message = tmp;
	}
	close(tube[0]);
	return (message);
}

char	*pipe_execute(t_env	*env, char *cmd)
{
	t_token	**tokens;
	int		fd;

	tokens = create_tokens(cmd);
	if (tokens == NULL)
		return (ft_strdup("stderr"));
	env_expand(env->local, tokens);
	wildcard_expand(&tokens);
	show_ctl(1);
	if (tokens[0]->value)
	{
		fd = exec_tokens(tokens, env);
	}
	else
		g_process.code = 0;
	free_tokens(tokens);
	// TODO: #26 Lire le filedesc de stdout
	return (ft_strdup("sdout"));
}

void	pipe_parse(t_env *env, char *cmd)
{
	char	**subcmds;
	char	*out;
	size_t	i;
	int		*tube;

	subcmds = pipe_split(cmd);
	i = 0;
	while (subcmds[i] != NULL)
	{
		tube = malloc(2 * sizeof(int));
		pipe(tube);
		if (fork() == 0)
		{
			out = pipe_execute(env, subcmds[i]);
			pipe_write(tube, out, ft_strlen(out));
			free(out);
			exit(EXIT_SUCCESS);
		}
		free(subcmds[i]);
		out = pipe_read(tube);
		printf("%s\n", out);
		free(tube);
		i++;
	}
	free(subcmds);
}
