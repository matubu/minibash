/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:48:12 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/11 22:14:08 by matubu           ###   ########.fr       */
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

static int	ispartofenv(char c)
{
	if ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z')
		|| (c == '_'))
		return (1);
	return (0);
}

int	isenvdefine(char *s)
{
	if (!ispartofenv(*s++))
		return (0);
	while (ispartofenv(*s))
		s++;
	if (*s == '=')
		return (1);
	return (0);
}

static char	*env_var(char *s, int *i)
{
	int		len;
	char	*env_name;

	len = 0;
	if (s[*i] == '$' || s[*i] == '*' || (s[*i] >= '0' && s[*i] <= '9'))//TODO find other one char variable case
		return (env_join(s, "\"one char variable name special case\"", i, 1));
	while (ispartofenv(s[*i + len]))
		len++;
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
