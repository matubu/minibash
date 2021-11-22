/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orand.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 08:40:19 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/22 14:33:13 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*substr(char *s, int n)
{
	char	*out;

	out = malloc((n + 1) * sizeof(char));
	out[n] = '\0';
	while (n--)
		out[n] = s[n];
	return (out);
}

char	*orand(t_env *env, char *s, int exec, int brace)
{
	int	n;
	int	m;

	while (*s && *s != ')')
	{
		while (is_space(*s))
			s++;
		n = -1;
		while (s[++n] && s[n] != '(' && s[n] != ')'
			&& !(s[n] == '&' && s[n + 1] == '&')
			&& !(s[n] == '|' && s[n + 1] == '|'))
		{
			if (s[n] == '"' || s[n] == '\'')
			{
				m = n;
				while (s[++n] && s[n] != s[m])
					;
			}
		}
		if (n && exec)
			pipe_parse(env, substr(s, n));
		s += n;
		if ((*s == '&' && s[1] == '&')
			|| (*s == '|' && s[1] == '|'))
		{
			if (((*s == '&' && g_process.code)
					|| (*s == '|' && !g_process.code)))
				exec = 0;
			s += 2;
		}
		while (is_space(*s))
			s++;
		if (*s == '(')
		{
			s = orand(env, s + 1, exec, brace);
			if (s == NULL || (*s++ != ')'
					&& err("syntax error unclosed token", "(")))
				return (NULL);
		}
	}
	if ((!brace && *s == ')' && err("syntax error near unexpected token", ")"))
		|| (brace && *s != ')' && err("syntax error expected token", ")")))
		return (NULL);
	return (s + brace);
}
