/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 10:48:05 by acoezard          #+#    #+#             */
/*   Updated: 2021/12/01 12:26:19 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_heredocs(t_redirection *redirs, char **buffer)
{
	char	*line;
	char	*tmp;

	*buffer = NULL;
	while (redirs->value)
	{
		if (*redirs->value && redirs->type == REDIR_HD_LEFT)
		{
			if (*buffer)
				free(*buffer);
			*buffer = ft_strdup("");
			line = readline("> ");
			while (line && ft_strcmp(redirs->value + 1, line))
			{
				tmp = ft_strjoin(*buffer, line);
				free(*buffer);
				*buffer = ft_strjoin(tmp, "\n");
				free(tmp);
				free(line);
				line = readline("> ");
			}
			free(line);
		}
		redirs++;
	}
}
