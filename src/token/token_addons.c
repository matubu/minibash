/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_addons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:56:25 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/25 15:17:38 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_tokens(t_token **tokens)
{
	int	i;
	int	j;

	i = -1;
	while (tokens[++i])
	{
		j = 0;
		while (tokens[i][j].value)
			free(tokens[i][j++].value);
		free(tokens[i]);
	}
	free(tokens);
	return (1);
}

char	**create_argv(char *cmd, t_env *env)
{
	char	**argv;
	t_token	**tokens;

	tokens = create_tokens(cmd);
	if (tokens == NULL || *tokens == NULL)
	{
		free_tokens(tokens);
		return (NULL);
	}
	env_expand(env->local, tokens);
	wildcard_expand(&tokens);
	if (tokens[0]->value == NULL)
	{
		free_tokens(tokens);
		return (NULL);
	}
	argv = token_to_argv(tokens);
	free_tokens(tokens);
	return (argv);
}
