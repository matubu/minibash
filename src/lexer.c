/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:50 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/17 15:17:23 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isoperator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

static int	tokenize(char *s, void (*token)(char *s, int n, void *arg),
		void *arg)
{
	int	n;
	int	m;

	n = 0;
	while (1)
	{
		if (s[n] == '"' || s[n] == '\'')
		{
			m = n;
			while (s[++n] != s[m])
				if (s[n] == '\0')
					return (err("syntax error unclosed token", s));
			n++;
		}
		else if (s[n] && s[n] != ' ' && !isoperator(s[n]))
			while (s[++n] && s[n] != ' ' && !isoperator(s[n]))
				;
		if (isoperator(s[n]) || s[n] == ' ' || s[n] == '\0')
		{
			token(s, n, arg);
			if (s[n] == '\0')
				return (0);
			if (isoperator(s[n]))
				token(s + n, 1, arg);
			s += n + 1;
			n = 0;
		}
	}
}

static void	inc(char *s, int n, void *arg)
{
	(void)s;
	if (n > 0)
		(*(int *)arg)++;
}

static void	sub_tokenize(char *s, int n, void (*sub)(char *s, int n, void *arg),
		void *arg)
{
	int	len;

	while (n > 0)
	{
		if (*s == '"' || *s == '\'')
		{
			len = 0;
			while (n-- > 0 && s[++len] != *s)
				;
			sub(s++, len - 1, arg);
			s += len;
			n--;
		}
		else
		{
			len = 0;
			while (s[len] != '"' && s[len] != '\'' && n-- > 0)
				len++;
			sub(s, len, arg);
			s += len;
		}
	}
}

static void	sub_fill(char *s, int n, t_token *arg)
{
	if (n <= 0)
		return ;
	while (arg->value)
		arg++;
	arg->expendable = *s;
	if (*s == '\'' || *s == '"')
		s++;
	arg->value = malloc((n + 1) * sizeof(char));
	arg->value[n] = '\0';
	while (n--)
		arg->value[n] = s[n];
	(++arg)->value = NULL;
}

static void	fill(char *s, int n, t_token **arg)
{
	int	len;

	if (n <= 0)
		return ;
	while (*arg)
		arg++;
	len = 1;
	sub_tokenize(s, n, inc, &len);
	*arg = malloc(len * sizeof(t_token));
	if (*arg == NULL)
		return ;
	(*arg)->value = NULL;
	sub_tokenize(s, n, (void (*)(char *, int, void *))sub_fill, *arg);
	*++arg = NULL;
}

// ./a.out      >    "test"  'hello'
// [[./a.out], [>], ["test", 'hello']]

// TODO: #20 a| b with no space
t_token	**create_tokens(char *s)
{
	int		len;
	t_token	**tokens;

	len = 1;
	if (tokenize(s, inc, &len) == -1)
	{
		g_process.code = 1;
		return (NULL);
	}
	tokens = malloc(len * sizeof(t_token *));
	if (tokens == NULL)
	{
		g_process.code = 1;
		return (NULL);
	}
	*tokens = NULL;
	tokenize(s, (void (*)(char *, int, void *))fill, tokens);
	return (tokens);
}
