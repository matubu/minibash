/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matubu <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 21:47:13 by matubu            #+#    #+#             */
/*   Updated: 2021/11/15 09:49:07 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ispartofenv(char c)
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

char	**env_get(char **env, char *kv)
{
	char	*ln;
	int		i;

	if (env == NULL)
		return (NULL);
	while (*env)
	{
		ln = *env;
		i = 0;
		while (*ln)
			if (!ispartofenv(kv[i]) && !ispartofenv(*ln))
				return (env);
		else if (*ln++ != kv[i++])
			break ;
		env++;
	}
	return (NULL);
}

void	env_set(char ***env, char *kv)
{
	int		len;
	char	**new;

	new = env_get(*env, kv);
	if (new)
	{
		*new = ft_strdup(kv);
		return ;
	}
	len = 1;
	if (*env)
		while ((*env)[len - 1])
			len++;
	new = malloc((len + 1) * sizeof(char *));
	new[len] = NULL;
	new[--len] = ft_strdup(kv);
	while (len--)
		new[len] = (*env)[len];
	if (*env)
		free(*env);
	*env = new;
}
