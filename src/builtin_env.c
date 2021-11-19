/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 11:40:02 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/19 11:34:25 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_builtin(char **argv, t_env *env)
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

void	unset_builtin(char **argv, t_env *env)
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
void	set_builtin(char **argv, t_env *env)
{
	printf("set %s\n", argv[0]);
	while (*argv)
	{
		env_set(&env->local, *argv);
		if (env_get(env->exported, *argv))
			env_set(&env->exported, *argv);
		argv++;
	}
}
