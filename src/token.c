/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 12:13:16 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/16 18:35:52 by matubu           ###   ########.fr       */
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
	if (res == NULL)
		return (NULL);
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

void	free_argv(char **argv)
{
	int	i;

	i = -1;
	while (argv[++i])
		free(argv[i]);
	free(argv);
}
