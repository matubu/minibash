/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orand.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 08:40:19 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/24 17:18:35 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char *s, int n)
{
	char	*out;

	out = malloc((n + 1) * sizeof(char));
	out[n] = '\0';
	while (n--)
		out[n] = s[n];
	return (out);
}

static char	*orand_return(int brace, char *s)
{
	if ((!brace && *s == ')' && err("syntax error near unexpected token", ")"))
		|| (brace && *s != ')' && err("syntax error expected token", ")")))
		return (NULL);
	return (s + brace);
}

static char	*orand_sub1(char *s, int *n, int *m)
{
	while (is_space(*s))
		s++;
	*n = -1;
	while (s[++*n] && s[*n] != '(' && s[*n] != ')' \
		&& !(s[*n] == '&' && s[*n + 1] == '&') \
		&& !(s[*n] == '|' && s[*n + 1] == '|'))
	{
		if (s[*n] == '"' || s[*n] == '\'')
		{
			*m = *n;
			while (s[++*n] && s[*n] != s[*m])
				;
		}
	}
	return (s);
}

static char	*orand_sub2(t_env *env, char *s, int *exec, int *n)
{
	if (n && *exec)
		pipe_parse(env, ft_substr(s, *n));
	s += *n;
	if ((*s == '&' && s[1] == '&') || (*s == '|' && s[1] == '|'))
	{
		if (((*s == '&' && g_process.code) \
			|| (*s == '|' && !g_process.code)))
			*exec = 0;
		s += 2;
	}
	while (is_space(*s))
		s++;
	return (s);
}

char	*orand(t_env *env, char *s, int exec, int brace)
{
	int	n;
	int	m;

	while (*s && *s != ')')
	{
		s = orand_sub1(s, &n, &m);
		if (n == 0 && *s != '(' && err("syntax error near unexpected token", s))
			return (NULL);
		s = orand_sub2(env, s, &exec, &n);
		if (*s == '(')
		{
			s = orand(env, s + 1, exec, brace);
			if (s == NULL || (*s++ != ')'
					&& err("syntax error unclosed token", "(")))
				return (NULL);
		}
	}
	return (orand_return(brace, s));
}
