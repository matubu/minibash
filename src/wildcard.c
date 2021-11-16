/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:01:16 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/16 20:02:20 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_pattern(char *pattern, char *file)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			if (pattern[1] == '\0')
				return (1);
			while (*file && *file != pattern[1])
				file++;
		}
		else if (*file++ != *pattern)
			return (0);
		pattern++;
	}
	return (*file == '\0');
}

static void	wildcard_replace(t_token *token)
{
	DIR				*dir;
	struct dirent	*file;

	(void)token;
	printf("------------ wildcard\n");
	dir = opendir(".");
	if (dir == NULL)
	{
		printf("no dir put error ?\n");
		return ;
	}
	while (1)
	{
		file = readdir(dir);
		if (file == NULL)
			break ;
		if (check_pattern(token->value, file->d_name))
			printf("follow pattern: insert me: %s\n", file->d_name);
		else
			printf("do not follow pattern: %s\n", file->d_name);
	}
	closedir(dir);
}

void	wildcard_expand(t_token **tokens)
{
	while (*tokens)
	{
		printf("->>> %s %s\n", tokens[0][0].value, tokens[0][1].value);
		if ((*tokens)[1].value == NULL && (*tokens)->expendable != '"'
				&& (*tokens)->expendable != '\''
				&& ft_strchr((*tokens)->value, '*'))
			wildcard_replace(*tokens);
		tokens++;
	}
}
