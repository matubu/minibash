/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:38:39 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/22 18:36:48 by mberger-         ###   ########.fr       */
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
	arg[1].value = ft_substr(s, n);
	if (n == 2 && s[1] == '<')
		arg[1].type = REDIR_HD_LEFT;
	else if (n == 2 && s[1] == '>')
		arg[1].type = REDIR_HD_RIGHT;
	else if (n == 1 && *s == '<')
		arg[1].type = REDIR_LEFT;
	else
		arg[1].type = REDIR_RIGHT;
	arg[1].expanded = 0;
	arg[2].value = NULL;
	return (0);
}

static int	redir_fill(char *s, int n, t_redirection *arg)
{
	char			*tmp;
	t_redirection	*start;
	int				i;

	(void)arg;
	if (n <= 0)
		return (0);
	start = arg;
	while (arg->value && arg[1].value)
		arg++;
	if (isredir(s, n))
		return (redir_process(s, n, arg));
	if (arg->expanded)
		arg = start;
	tmp = malloc((n + ft_strlen(arg->value) + 2) * sizeof(char));
	i = -1;
	while (arg->value[++i])
		tmp[i] = arg->value[i];
	tmp[i] = ' ';
	while (n-- && *s != ' ')
		tmp[++i] = *s++;
	tmp[++i] = '\0';
	free(arg->value);
	arg->value = tmp;
	arg->expanded = 1;
	printf("Value: %s | Type: %x\n\n", arg->value, arg->type);
	return (0);
}

t_redirection	*exec_redirections(char *cmd, t_env *env)
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
	redirs[0].type = REDIR_NOT;
	redirs[1].value = NULL;
	if (tokenize(cmd, (int (*)()) redir_fill, redirs))
	{
		free(redirs);
		return (NULL);
	}
	return (redirs);
}
