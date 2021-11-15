/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:01:16 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/15 16:30:49 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wildcard_replace(t_token *tokens)
{
	printf("%s\n", tokens->value);
}

void	wildcard_expand(t_token *tokens)
{
	while (tokens->value)
	{
		if (ft_strchr(tokens->value, '*'))
			wildcard_replace(tokens);
		tokens++;
	}
}
