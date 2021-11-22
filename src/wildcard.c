/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:01:16 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/22 10:52:04 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	**insert(t_token **tokens, int i, char *filename)
{
	int		n;
	t_token	**res;

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
	while (*file && *pattern)
	{
		if (*pattern == '*' && ++pattern)
		{
			while (*file)
				if (check_pattern(pattern, file++))
					return (1);
		}
		else if (*file++ != *pattern++)
			return (0);
	}
	return (*file == '\0' && *pattern == '\0');
}

static void	wildcard_process(t_token ***tokens, struct dirent *file,
	int *i, int *count)
{
	if ((*count)++)
		*tokens = insert(*tokens, (*i)++, file->d_name);
	else
		(*tokens)[*i]->value = ft_strdup(file->d_name);
}

static void	wildcard_replace(t_token ***tokens, int *i)
{
	DIR				*dir;
	struct dirent	*file;
	int				count;
	char			*pattern;

	dir = opendir(".");
	pattern = (*tokens)[*i]->value;
	if (dir == NULL)
		return ;
	count = 0;
	while (1)
	{
		file = readdir(dir);
		if (file == NULL)
			break ;
		if (!(*pattern == '*' && *file->d_name == '.')
			&& check_pattern(pattern, file->d_name))
			wildcard_process(tokens, file, i, &count);
	}
	if (count)
		free(pattern);
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
