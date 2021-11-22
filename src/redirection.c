/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:38:39 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/22 13:59:22 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	isredir(char *s, int n)
{
	if (n == 1 && (s[0] == '<' || s[0] == '>'))
		return (1);
	if (n == 2 && ((*s == '<' && s[1] == '<') || (*s == '>' && s[1] == '>')))
		return (1);
	return (0);
}

static int	redir_inc(char *s, int n, void *arg)
{
	if (isredir(s, n))
		(*(int *) arg)++;
	return (0);
}

static int	redir_process(char *s, int n, t_redirection *arg)
{
	if (s[n] == '\0')
		return (err("syntax error near unexpected token", ">"));
	s[n] = '\0';
	printf("TOKEN TROUVE | Size=%d\n", n);
	arg[1].value = ft_strdup(s);
	if (n == 2 && s[1] == '<')
		arg[1].type = REDIR_HD_LEFT;
	else if (n == 2 && s[1] == '>')
		arg[1].type = REDIR_HD_RIGHT;
	else if (n == 1 && *s == '<')
		arg[1].type = REDIR_LEFT;
	else
		arg[1].type = REDIR_RIGHT;
	arg[2].value = NULL;
	return (0);
}

static int	redir_fill(char *s, int n, t_redirection *arg)
{
	char	*tmp;
	int		i;

	if (n <= 0)
		return (0);
	while (arg->value && arg[1].value)
		arg++;
	if (isredir(s, n))
		return (redir_process(s, n, arg));
	if (*s == '\'' || *s == '"')
		n++;
	tmp = malloc((n + ft_strlen(arg->value) + 2) * sizeof(char));
	i = -1;
	while (arg->value[++i])
	{
		tmp[i] = arg->value[i];
		printf("%c\n", arg->value[i]);
	}
	tmp[i] = ' ';
	while (n--)
		tmp[++i] = *s++;
	tmp[++i] = '\0';
	free(arg->value);
	arg->value = tmp;
	arg->type = REDIR_NOT;
	return (0);
}

int	exec_redirections(char *cmd, t_env *env)
{
	t_redirection	*redirs;
	int				len;

	(void) env;
	len = 2;
	if (tokenize(cmd, redir_inc, &len) == -1)
		return (0);
	redirs = malloc(len * sizeof(t_redirection));
	if (redirs == NULL)
		return (0);
	redirs[0].value = ft_strdup("");
	redirs[1].value = NULL;
	if (tokenize(cmd, (int (*)()) redir_fill, redirs))
	{
		free(redirs);
		return (0);
	}
	while (redirs->value)
	{
		printf("Value: %s | Type: %x\n", redirs->value, redirs->type);
		redirs++;
	}
	return (0);
}
