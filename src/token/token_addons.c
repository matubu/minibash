/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_addons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:56:25 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/24 16:57:43 by acoezard         ###   ########.fr       */
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
