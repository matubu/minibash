/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:48:12 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/11 19:51:51 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_validate(t_token *tokens)
{
	while (*(tokens->value))
	{
		if (*(tokens->value) == '$')
			return (1);
		tokens->value++;
	}
	return (0);
}

static char	*env_replace(char *value, char *env_value, int i, int j)
{
	char	*nvalue;
	int		nvalue_l;

	nvalue_l = ft_strlen(value) - (j - i + 1) + ft_strlen(env_value);
	nvalue = (char *) malloc(sizeof(char) * nvalue_l);

	ft_strlcpy(nvalue, value, i);
	ft_strcat(nvalue, env_value);
	ft_strcat(nvalue, value + j);

	//free(value);
	return (nvalue);
}

static char	*env_split(char *value, int i, int j)
{
	char	*env_name;
	char	*env;

	env_name = (char *) malloc((j - i + 1) * sizeof(char));
	ft_strlcpy(env_name, value + i, j);
	env = getenv(env_name);
	if (env == NULL)
		return (env_replace(value, "", i, j));
	return (env_replace(value, env, i, j));
}

static void	env_search(t_token *tokens)
{
	char	*ptr;
	int		i;
	int		j;

	while (tokens->value)
	{
		ptr = tokens->value;
		i = 0;
		while (ptr[i] && ptr[i] != '$')
			i++;
		if (!ptr[i] || !ptr[i + 1])
			break;
		if (ptr[i + 1] == '$')
			tokens->value += i + 2; // Afficher le PID du Shell
		else
		{
			j = i + 1;
			while (ptr[j] && ptr[j] != ' ' && ptr[j] != '$')
				j++;
			tokens->value = env_split(tokens->value, i, j);
			tokens->value += j;
		}
	}
}

void	env_expend(t_token *tokens)
{
	while (tokens->value)
	{
		if (env_validate(tokens))
			env_search(tokens);
		tokens++;
	}
}
