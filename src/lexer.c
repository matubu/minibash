/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:50 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/29 14:27:43 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenize(char *s, int (*token)(), void *arg)
{
	int	n;
	int	m;

	while (*s)
	{
		while (is_space(*s))
			s++;
		n = 0;
		while (s[n] && !is_space(s[n]) && !is_operator(s[n]))
		{
			m = n;
			if (s[m] == '"' || s[m] == '\'')
				while (s[++n] != s[m])
					if (s[n] == '\0')
						return (err("syntax error unclosed token", s, 258));
			n++;
		}
		token(s, n, arg);
		m = n;
		while (is_operator(s[m]))
			m++;
		token(s + n, m - n, arg);
		s += m;
	}
	return (0);
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
			sub(s, len, arg);
			s += len + 1;
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
	if ((*s == '\'' || *s == '"') && n--)
		s++;
	arg->value = malloc((n + 1) * sizeof(char));
	arg->value[n] = '\0';
	while (n--)
		arg->value[n] = s[n];
	(++arg)->value = NULL;
}

static int	fill(char *s, int n, t_token **arg)
{
	int	len;

	if (n <= 0)
		return (0);
	while (*arg)
		arg++;
	len = 1;
	sub_tokenize(s, n, (void (*)()) basic_inc, &len);
	*arg = malloc(len * sizeof(t_token));
	if (*arg == NULL)
		return (0);
	(*arg)->value = NULL;
	sub_tokenize(s, n, (void (*)()) sub_fill, *arg);
	*++arg = NULL;
	return (0);
}

t_token	**create_tokens(char *s)
{
	int		len;
	t_token	**tokens;

	len = 1;
	if (tokenize(s, basic_inc, &len) == -1)
		return (NULL);
	tokens = malloc(len * sizeof(t_token *));
	if (tokens == NULL)
		return (NULL);
	*tokens = NULL;
	tokenize(s, (int (*)()) fill, tokens);
	return (tokens);
}
