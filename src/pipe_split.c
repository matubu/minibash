/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:38:39 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/22 08:27:45 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	inc(char *s, int n, void *arg)
{
	if (n == 1 && *s == '|')
		(*(int *)arg)++;
	return (0);
}

static int	fill(char *s, int n, char **arg)
{
	char	*tmp;
	int		i;

	if (n <= 0)
		return (0);
	while (*arg && arg[1])
		arg++;
	if (*s == '|')
	{
		if (n != 1 || **arg == '\0')
			return (err("syntax error near unexpected token", "|"));
		*++arg = ft_strdup("");
		*++arg = NULL;
		return (0);
	}
	if (*s == '\'' || *s == '"')
		n++;
	tmp = malloc((n + ft_strlen(*arg) + 2) * sizeof(char));
	i = -1;
	while ((*arg)[++i])
		tmp[i] = (*arg)[i];
	tmp[i] = ' ';
	while (n--)
		tmp[++i] = *s++;
	tmp[++i] = '\0';
	free(*arg);
	*arg = tmp;
	return (0);
}

char	**pipe_split(char *s)
{
	int		len;
	char	**pipes;

	len = 2;
	if (tokenize(s, inc, &len) == -1)
		return (NULL);
	pipes = malloc(len * sizeof(char *));
	if (pipes == NULL)
		return (NULL);
	*pipes = ft_strdup("");
	pipes[1] = NULL;
	if (tokenize(s, (int (*)(char *, int, void *))fill, pipes))
	{
		free_argv(pipes);
		return (NULL);
	}
	return (pipes);
}
