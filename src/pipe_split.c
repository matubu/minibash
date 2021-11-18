/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:38:39 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/18 17:46:58 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
static int	pipe_split_sep(char c)
{
	return (c == '|' || c == '\0');
}

static int	pipe_split_search(const char *s)
{
	int		j;

	j = 0;
	while (!pipe_split_sep(s[j]))
		j++;
	return (j);
}

static int	pipe_split_size(const char *s)
{
	int		size;
	int		i;

	size = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (!pipe_split_sep(s[i]) && pipe_split_sep(s[i + 1]))
			size++;
		i++;
	}
	return (size);
}

static char	*pipe_split_copy(const char *s, int start, int size)
{
	char	*word;
	int		i;

	word = (char *) malloc((size + 1) * sizeof(char));
	i = 0;
	while (i < size)
	{
		word[i] = s[start + i];
		i++;
	}
	word[i] = 0;
	return (word);
}

char	**pipe_split(char *s)
{
	char	**words;
	int		size;
	int		inc[3];

	size = pipe_split_size(s);
	words = (char **) malloc(sizeof(char *) * (size + 1));
	if (words == NULL)
		return (NULL);
	inc[0] = 0;
	inc[2] = 0;
	while (s[inc[0]] != '\0')
	{
		// TODO: #28 Gerer le cas des quotes pour les pipes
		if (s[inc[0]] == '"' || s[inc[0]] == '\'')
		{
			inc[1] = inc[0];
			while (s[++inc[0]] != s[inc[1]])
			{
				if (s[inc[0]] == '\0')
				{
					err("syntax error near unexpected token", s + inc[0]);
					free(words);
					return (NULL);
				}
			}
			++inc[0];
			continue ;
		}
		if (s[inc[0]] == '|')
		{
			if (inc[0]++ == 0 || s[inc[0]] == '\0' || s[inc[0]] == '|')
			{
				err("syntax error near unexpected token", "|");
				free(words);
				return (NULL);
			}
		}
		else
		{
			inc[1] = pipe_split_search(s + inc[0]);
			words[inc[2]++] = pipe_split_copy(s, inc[0], inc[1]);
			inc[0] += inc[1];
		}
	}
	words[size] = NULL;
	return (words);
}
*/

static void	inc(char *s, int n, void *arg)
{
	(void)s;
	if (n > 0 && *s == '|' && s[1] == '\0')
		(*(int *)arg)++;
}

static void	fill(char *s, int n, char **arg)
{
	char	*tmp;
	int		i;

	if (n <= 0)
		return ;
	while (*arg && arg[1])
		arg++;
	if (*s == '|' && s[1] == '\0')
	{
		if (**arg == '\0')
		{
			err("syntax error near unexpected token", "|");
			return ;
		}
		*++arg = ft_strdup("");
		*++arg = NULL;
		return ;
	}
	tmp = malloc(ft_strlen(s) + ft_strlen(*arg) + 2);
	i = -1;
	while ((*arg)[++i])
		tmp[i] = (*arg)[i];
	tmp[i] = ' ';
	while (n--)
		tmp[++i] = *s++;
	tmp[i] = '\0';
	free(*arg);
	*arg = tmp;
}

char	**pipe_split(char *s)
{
	int		len;
	char	**pipes;

	len = 2;
	if (tokenize(s, inc, &len) == -1)
	{
		g_process.code = 1;
		return (NULL);
	}
	pipes = malloc(len * sizeof(char));
	if (pipes == NULL)
	{
		g_process.code = 1;
		return (NULL);
	}
	*pipes = ft_strdup("");
	pipes[1] = NULL;
	tokenize(s, (void (*)(char *, int, void *))fill, pipes);
	return (pipes);
}
