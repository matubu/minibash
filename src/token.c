/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 12:13:16 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/15 17:55:54 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].value)
		free(tokens[i++].value);
	free(tokens);
	return (1);
}

char	**token_to_argv(t_token *tokens)
{
	char	**argv;
	int		len;

	len = 0;
	while (tokens[len++].value)
		;
	argv = malloc(len * sizeof(char *));
	if (argv == NULL)
		return (NULL);
	argv[--len] = NULL;
	while (len--)
		argv[len] = tokens[len].value;
	return (argv);
}
