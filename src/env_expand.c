/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:48:12 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/16 13:17:52 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*env_join(char *s, char *value, int *i, int len)
{
	char	*out;
	int		value_len;

	if (value++ == NULL)
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

static char	*env_var(char **env, char *s, int *i)
{
	int		len;
	char	**kv;

	len = 0;
	if (s[*i] == '?')
		return (env_join(s, itoa_buf(g_process.code), i, 1));
	while (ispartofenv(s[*i + len]))
		len++;
	kv = env_get(env, s + *i);
	if (kv == NULL)
		return (s);
	return (env_join(s, ft_strchr(*kv, '='), i, len));
}

static char	*env_replace(char **env, char *s)
{
	int		i;

	i = -1;
	while (s[++i])
	{
		if (s[i] == '$')
		{
			if (s[++i] == '\0' || s[i] == ' ')
				continue ;
			s = env_var(env, s, &i);
		}
	}
	return (s);
}

void	env_expand(char **env, t_token **tokens)
{
	int	i;

	while (*tokens)
	{
		i = -1;
		while ((*tokens)[++i].value)
			if ((*tokens)[i].expendable != '\'')
				(*tokens)[i].value = env_replace(env, (*tokens)[i].value);
		tokens++;
	}
}
