/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 10:48:05 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/23 14:36:34 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_heredocs(t_redirection *redir)
{
	char	*buffer;
	char	*line;
	char	*tmp;

	buffer = ft_strdup("");
	line = readline("> ");
	while (line != NULL && ft_strcmp(redir->value + 1, line))
	{
		tmp = ft_strjoin(buffer, line);
		free(buffer);
		buffer = ft_strjoin(tmp, "\n");
		free(tmp);
		free(line);
		line = readline("> ");
	}
	free(line);
	return (buffer);
}

int	exec_heredocs(t_redirection *redirs, char **buffer)
{
	*buffer = ft_strdup("");
	while (redirs->value)
	{
		if (*redirs->value && redirs->type == REDIR_HD_LEFT)
		{
			free(*buffer);
			*buffer = read_heredocs(redirs);
		}
		redirs++;
	}
	return (0);
}
