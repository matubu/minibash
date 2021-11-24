/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_addons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 10:57:47 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/24 18:02:54 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*tokenize_iterate(char *s, int *n)
{
	while (s[++*n] && !is_space(s[*n]) && !is_operator(s[*n]) \
		&& s[*n] != '"' && s[*n] != '\'')
		;
	return (s);
}

static int	tokenize_check(char *s, int *n, int *m)
{
	*m = *n;
	while (s[++*n] != s[*m])
		if (s[*n] == '\0')
			return (err("syntax error unclosed token", s));
	(*n)++;
	return (0);
}

static int	tokenize_part1(char *s, int *n, int *m)
{
	if (s[*n] == '\0')
		return (0);
	*m = *n;
	while (is_operator(s[*n]))
		(*n)++;
	return (1);
}

static char	*tokenize_part2(char *s, int *n)
{
	s += *n;
	while (is_space(*s))
		s++;
	*n = 0;
	return (s);
}

int	tokenize(char *s, int (*token)(), void *arg)
{
	int	n;
	int	m;

	n = 0;
	while (1)
	{
		if ((s[n] == '"' || s[n] == '\'') && tokenize_check(s, &n, &m) == -1)
			return (-1);
		else if (s[n] && !is_space(s[n]) && !is_operator(s[n]))
			s = tokenize_iterate(s, &n);
		if (is_operator(s[n]) || is_space(s[n]) || s[n] == '\0')
		{
			if (token(s, n, arg))
				return (1);
			if (!tokenize_part1(s, &n, &m))
				return (0);
			if (token(s + m, n - m, arg))
				return (1);
			s = tokenize_part2(s, &n);
		}
	}
}
