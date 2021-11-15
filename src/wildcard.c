/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:01:16 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/15 17:31:24 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wildcard_replace(t_token *token)
{
	token->expendable = 0;
	printf("%s\n", token->value);
	// TODO: #10 remplacer les * par leur valeur
}

void	wildcard_expand(t_token *tokens)
{
	while (tokens->value)
	{
		// TODO: #9 Verifier si il y a des ''
		if (ft_strchr(tokens->value, '*'))
			wildcard_replace(tokens);
		tokens++;
	}
}
