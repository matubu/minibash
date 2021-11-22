/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:38:39 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/22 08:40:51 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	isredir(char *s, int n)
{
	return ((n == 1 && (*s == '<' || *s == '>')) || (n == 2
		&& ((*s == '<' && s[1] == '<' ) || (*s == '>' && s[1] == '>'))));
}

static int	redir_inc(char *s, int n, void *arg)
{
	if (isredir(s, n))
		(*(int *)arg)++;
	return (0);
}

static int	redir_fill(char *s, int n, char **arg)
{
	char	*tmp;
	int		i;

	if (n <= 0)
		return (0);
	while (*arg && arg[1])
		arg++;
	if (isredir(s, n))
	{
		if (s[n] == '\0')
			return (err("syntax error near redirection token", ""));
		arg[1] = ft_strdup("");
		arg[2] = NULL;
		printf("TOKEN TROUVE\n");
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

char	**redir_split(char *s)
{
	int		len;
	char	**redirections;

	len = 2;
	if (tokenize(s, redir_inc, &len) == -1)
		return (NULL);
	redirections = malloc(len * sizeof(char *));
	if (redirections == NULL)
		return (NULL);
	*redirections = ft_strdup("");
	redirections[1] = NULL;
	if (tokenize(s, (int (*)(char *, int, void *)) redir_fill, redirections))
	{
		free_argv(redirections);
		return (NULL);
	}
	while (*redirections)
		printf("%s\n", *(redirections++));
	return (redirections);
}
