/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orand.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 08:40:19 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/25 10:10:14 by mberger-         ###   ########.fr       */
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

char	*orand_exec(t_env *env, char *s, int exec)
{
	int	n;
	int	m;

	if (*s == '(')
		return (orand(env, s + 1, exec, 1));
	else
	{
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
			pipe_parse(env, ft_substr(s, n));
		else if (n == 0 && err("syntax error expected token", s))
			return (NULL);
		return (s + n);
	}
}

char	*orand(t_env *env, char *s, int exec, int brace)
{
	while (*s && *s != ')')
	{
		while (is_space(*s))
			s++;
		s = orand_exec(env, s, exec);
		if (s == NULL)
			return (NULL);
		while (is_space(*s))
			s++;
		if ((*s == '&' && s[1] == '&') || (*s == '|' && s[1] == '|'))
		{
			if (((*s == '&' && g_process.code)
					|| (*s == '|' && !g_process.code)))
				exec = 0;
			s += 2;
		}
		else if (*s != '\0' && *s != ')'
			&& err("syntax error expected token", s))
			return (NULL);
	}
	if ((!brace && *s == ')' && err("syntax error near unexpected token", ")"))
		|| (brace && *s != ')' && err("syntax error expected token", ")")))
		return (NULL);
	return (s + brace);
}
