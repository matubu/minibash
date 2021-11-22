/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 11:40:02 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/22 10:27:55 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_builtin(char **argv, t_env *env)
{
	char	**found;

	while (*argv)
	{
		if (isenvdefine(*argv))
		{
			env_set(&env->local, *argv);
			env_set(&env->exported, *argv++);
			continue ;
		}
		found = env_get(env->local, *argv++);
		if (found)
			env_set(&env->exported, *found);
	}
	g_process.code = 0;
}

void	unset_builtin(char **argv, t_env *env)
{
	char	**found;

	while (*argv)
	{
		found = env_get(env->local, *argv);
		if (found)
			**found = '\0';
		found = env_get(env->exported, *argv++);
		if (found)
			**found = '\0';
	}
	g_process.code = 0;
}

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
	g_process.code = 0;
}
