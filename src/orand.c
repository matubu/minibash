/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orand.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 08:40:19 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/22 09:53:32 by mberger-         ###   ########.fr       */
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

//void	pipe_parse(t_env *env, char *cmd)

void	orand(t_env *env, char *s)
{
	int	n;
	int	m;
	int	brace;

	brace = *s == '(' && s++;
	while (*s && *s != ')')
	{
		n = -1;
		while (s[++n]
				&& s[n] != '('
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
		pipe_parse(env, substr(s, n));
		s += n;
		//if (*s == '(')
		//	err("");
		/*else */
		if ((*s == '&' && s[1] == '&')
			|| (*s == '|' && s[1] == '|'))
		{
			if (((*s == '&' && !g_process.code)
				|| (*s == '|' && g_process.code)))
				break ;
			s += 2;
		}
		while (is_space(*s))
			s++;
		//if (*s == '(')
		//	orand(env, s++);
	}
	//if (*s == ')')
	//	err("");
}
