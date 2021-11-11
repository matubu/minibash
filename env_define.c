/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_define.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matubu <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 21:47:13 by matubu            #+#    #+#             */
/*   Updated: 2021/11/11 22:19:28 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *getenvdefinevalue(char *cmd)
{
	char	*s;
	int		len;

	len = ft_strlen(cmd) + 1;
	s = malloc(len * sizeof(char));
	ft_strlcpy(s, cmd, len);
	return (s);
}

//TODO fix will produce leaks
void	envdefine(char ***env, char *cmd)
{
	int		len;
	char	**new;

	len = 0;
	while ((*env)[len++])
		;
	new = malloc((len + 1) * sizeof(char *));
	new[len] = NULL;
	new[--len] = getenvdefinevalue(cmd);
	while (len--)
		new[len] = (*env)[len];
	//free(*env);
	*env = new;
}
