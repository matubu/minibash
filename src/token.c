/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 12:13:16 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/24 16:57:07 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*token_join(t_token *token)
{
	int		len;
	char	*res;
	int		i;
	int		j;

	len = 0;
	i = -1;
	while (token[++i].value)
	{
		j = -1;
		while (token[i].value[++j])
			len++;
	}
	res = malloc((len + 1) * sizeof(char));
	res[len] = '\0';
	len = 0;
	i = -1;
	while (token[++i].value)
	{
		j = -1;
		while (token[i].value[++j])
			res[len++] = token[i].value[j];
	}
	return (res);
}

char	**token_to_argv(t_token **tokens)
{
	char	**argv;
	int		len;

	len = 0;
	while (tokens[len++])
		;
	argv = malloc(len * sizeof(char *));
	if (argv == NULL)
		return (NULL);
	argv[--len] = NULL;
	while (len--)
		argv[len] = token_join(tokens[len]);
	return (argv);
}

int	exec_tokens(char *cmd, t_env *env)
{
	char	**argv;
	int		ret;

	ret = 1;
	argv = create_argv(cmd, env);
	if (argv == NULL)
		return (1);
	runsearch(argv[0], argv, env->exported);
	free_argv(argv);
	return (ret);
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
