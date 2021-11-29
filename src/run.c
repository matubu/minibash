/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:38 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/29 14:02:28 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* will concatenate a path with a relative path
* @param {char *} path the absolute path
* @param {int} the path string length
* @param {char *} the path relative to path ex: cat
*/
char	*pathncat(char *path, int n, char *relative)
{
	char	*full;
	char	*start;

	full = malloc(n + ft_strlen(relative) + 2);
	if (full == NULL)
		return (NULL);
	start = full;
	while (n--)
		*full++ = *path++;
	*full++ = '/';
	while (*relative)
		*full++ = *relative++;
	*full = '\0';
	return (start);
}

/**
* will try to run a command from is full path
* @param {char *} path to executable
*/
int	runfrompath(char *cmd, char **argv, char **env)
{
	if (access(cmd, X_OK) == -1 || execve(cmd, argv, env) == -1)
		return (-1);
	return (0);
}

/**
* will search in PATH folders the command if not a relative or absolute path
*/
int	runsearch(char *cmd, char **argv, char **env)
{
	char	**path;
	char	*file;
	int		ret;
	int		len;

	if (*cmd == '.' || *cmd == '/')
	{
		if (runfrompath(cmd, argv, env) == -1)
			return (err(strerror(errno), cmd, errno));
		return (0);
	}
	path = env_get(env, "PATH");
	while (path && **path)
	{
		len = 0;
		while ((*path)[len] && (*path)[len] != ':')
			len++;
		file = pathncat(*path, len, cmd);
		ret = runfrompath(file, argv, env);
		free(file);
		if (ret != -1)
			return (0);
		(*path) += len + ((*path)[len] == ':');
	}
	return (err("command not found", cmd, 127));
}

int	exec_builtin_argv(char **argv, t_env *env, int stdout)
{
	if (isenvdefine(*argv))
		set_builtin(argv, env);
	else if (!ft_strcmp(*argv, "echo"))
		echo_builtin(stdout, argv + 1);
	else if (!ft_strcmp(*argv, "cd"))
		cd_builtin(env, argv + 1);
	else if (!ft_strcmp(*argv, "pwd"))
		pwd_builtin(stdout, argv + 1, env);
	else if (!ft_strcmp(*argv, "export"))
		export_builtin(stdout, argv + 1, env);
	else if (!ft_strcmp(*argv, "unset"))
		unset_builtin(argv + 1, env);
	else if (!ft_strcmp(*argv, "env"))
		env_builtin(stdout, env->exported);
	else if (!ft_strcmp(*argv, "exit"))
		exit_builtin(stdout, argv + 1);
	else
		return (1);
	return (0);
}

int	exec_builtin(char *cmd, t_env *env, int stdout)
{
	char	**argv;

	if (stdout == -1)
		return (1);
	argv = create_argv(cmd, env);
	if (argv == NULL)
		return (0);
	if (exec_builtin_argv(argv, env, stdout))
		return (free_argv(argv));
	if (stdout != 1)
		close(stdout);
	return (free_argv(argv) || 1);
}
