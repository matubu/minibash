/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_addons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 10:57:47 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/23 16:24:29 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\f'));
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	tokenize(char *s, int (*token)(), void *arg)
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
		else if (s[n] && !is_space(s[n]) && !is_operator(s[n]))
			while (s[++n] && !is_space(s[n]) && !is_operator(s[n])
					&& s[n] != '"' && s[n] != '\'')
				;
		if (is_operator(s[n]) || is_space(s[n]) || s[n] == '\0')
		{
			if (token(s, n, arg))
				return (1);
			if (s[n] == '\0')
				return (0);
			m = n;
			while (is_operator(s[n]))
				n++;
			if (token(s + m, n - m, arg))
				return (1);
			s += n;
			while (is_space(*s))
				s++;
			n = 0;
		}
	}
}
