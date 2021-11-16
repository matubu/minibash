/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:01:16 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/16 20:47:21 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	**insert(t_token **tokens, int i, char *filename)
{
	int		n;
	t_token	**res;

	printf("add %s\n", filename);
	n = -1;
	while (tokens[++n])
		;
	res = malloc((n + 2) * sizeof(char *));
	if (res == NULL)
		return (NULL);
	res[++n] = NULL;
	while (n--)
		if (n >= i)
			res[n + 1] = tokens[n];
	else
		res[n] = tokens[n];
	res[i] = malloc(sizeof(t_token) * 2);
	res[i][0].value = ft_strdup(filename);
	res[i][0].expendable = 0;
	res[i][1].value = NULL;
	free(tokens);
	return (res);
}

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

//TODO remove *.c
static void	wildcard_replace(t_token ***tokens, int *i)
{
	DIR				*dir;
	struct dirent	*file;

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
		if (check_pattern((*tokens)[*i]->value, file->d_name))
			*tokens = insert(*tokens, (*i)++, file->d_name);
	}
	closedir(dir);
}

void	wildcard_expand(t_token ***tokens)
{
	int	i;

	i = -1;
	while ((*tokens)[++i])
	{
		if ((*tokens)[i][1].value == NULL && (*tokens)[i]->expendable != '"'
				&& (*tokens)[i]->expendable != '\''
				&& ft_strchr((*tokens)[i]->value, '*'))
			wildcard_replace(tokens, &i);
	}
}
