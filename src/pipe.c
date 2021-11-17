/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:33:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/17 15:33:56 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*pipe_execute(t_env	*env, char *cmd)
{
	t_token	**tokens;

	tokens = create_tokens(cmd);
	if (tokens == NULL)
		return (NULL);
	env_expand(env->local, tokens);
	wildcard_expand(&tokens);
	show_ctl(1);
	if (tokens[0]->value)
		exec_tokens(tokens, env);
	else
		g_process.code = 0;
	free_tokens(tokens);
	return (NULL);
}

void	pipe_parse(t_env *env, char *cmd)
{
	char	**subcmds;
	char	*out;
	size_t	i;

	subcmds = pipe_split(cmd);
	i = 0;
	while (subcmds[i] != NULL)
	{
		out = pipe_execute(env, subcmds[i]);
		free(subcmds[i]);
		i++;
	}
	free(subcmds);
}
