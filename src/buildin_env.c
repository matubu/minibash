/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 11:40:02 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/15 13:57:20 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_buildin(char **argv, t_env *env)
{
	char	**found;

	while (*argv && *++argv)
	{
		if (isenvdefine(*argv))
		{
			env_set(&env->local, *argv);
			env_set(&env->exported, *argv);
			continue ;
		}
		found = env_get(env->local, *argv);
		if (found)
			env_set(&env->exported, *found);
	}
}

void	unset_buildin(char **argv, t_env *env)
{
	char	**found;

	while (*argv && *++argv)
	{
		found = env_get(env->local, *argv);
		if (found)
			**found = '\0';
		found = env_get(env->exported, *argv);
		if (found)
			**found = '\0';
	}
}

//TODO a=5 echo
void	set_buildin(char **argv, t_env *env)
{
	while (*argv)
	{
		env_set(&env->local, *argv);
		if (env_get(env->exported, *argv))
			env_set(&env->exported, *argv);
		argv++;
	}
}
