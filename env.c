/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:48:12 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/11 21:00:35 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*env_join(char *s, char *value, int *i, int len)
{
	char	*out;
	int		value_len;

	if (value == NULL)
		return (s);
	value_len = ft_strlen(value);
	out = malloc(sizeof(char) * (ft_strlen(s) + value_len - len + 1));
	ft_strlcpy(out, s, *i);
	ft_strcat(out, value);
	ft_strcat(out, s + *i + len);
	*i += len + 1;
	free(s);
	return (out);
}

static char *env_var(char *s, int *i)
{
	int		len;
	char	*env_name;

	len = 0;
	while (s[*i + len] && s[*i + len] != ' ' && s[*i + len++] != '$')
		;
	env_name = malloc((len + 1) * sizeof(char));
	ft_strlcpy(env_name, s + *i, len + 1);
	s = env_join(s, getenv(env_name), i, len);
	free(env_name);
	return (s);
}

static char	*env_replace(char *s)
{
	int		i;

	i = -1;
	while (s[++i])
		if (s[i] == '$')
		{
			if (s[++i] == '\0' || s[i] == ' ')
				continue ;
			s = env_var(s, &i);
		}
	return (s);
}

void	env_expend(t_token *tokens)
{
	while (tokens->value)
	{
		if (tokens->expendable)
			tokens->value = env_replace(tokens->value);
		tokens++;
	}
}
