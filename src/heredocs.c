/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 10:48:05 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/23 11:52:55 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_heredocs(t_redirection *redir)
{
	char	*buffer;
	char	*line;
	char	*tmp;

	buffer = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(redir->value + 1, line))
		{
			tmp = ft_strjoin(buffer, line);
			free(buffer);
			buffer = ft_strjoin(tmp, "\n");
			free(tmp);
			free(line);
			continue ;
		}
		free(line);
		break ;
	}
	return (buffer);
}

void	exec_heredocs(t_redirection *redirs)
{
	char	*buffer;

	buffer = ft_strdup("");
	while (redirs->value)
	{
		if (*redirs->value && redirs->type == REDIR_HD_LEFT)
		{
			free(buffer);
			buffer = read_heredocs(redirs);
		}
		redirs++;
	}
	printf("%s", buffer);
	free(buffer);
}
