/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:38 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/15 16:33:25 by acoezard         ###   ########.fr       */
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
	pid_t	pid;

	if (access(cmd, X_OK) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd, argv, env);
		exit(-1);
	}
	else
	{
		g_process.pid = pid;
		wait(&g_process.code);
	}
	return (0);
}

/**
* will search the command if not a relative or absulte path in the folders specified in then environment variable PATH
*/
//TODO permission denied error for command too ?
//TODO PATH=5 => ls => crash
int	runsearch(char *cmd, char **argv, char **env)
{
	char	*path;
	char	*file;
	int		ret;
	int		len;

	if (*cmd == '.' || *cmd == '/')
	{
		if (runfrompath(cmd, argv, env) == -1)
			return (err(strerror(errno), cmd));
		return (0);
	}
	path = *env_get(env, "PATH");
	while (*path)
	{
		len = 0;
		while (path[len] && path[len] != ':')
			len++;
		file = pathncat(path, len, cmd);
		ret = runfrompath(file, argv, env);
		free(file);
		if (ret != -1)
			return (0);
		path += len + (path[len] == ':');
	}
	return (err("command not found", cmd));
}

/**
* will execute custom function if one of the buildin
* else it will call runsearch
*/
//TODO return or set the exit code $?
//TODO parsing error = exit code 1
void	run(char *cmd, char **argv, t_env *env)
{
	if (argv == NULL)
		return ;
	if (isenvdefine(cmd))
		set_builtin(argv, env);
	else if (!ft_strcmp(cmd, "echo"))
		echo_builtin(argv + 1);
	else if (!ft_strcmp(cmd, "cd"))
		cd_builtin(argv);
	else if (!ft_strcmp(cmd, "pwd"))
		pwd_builtin(argv);
	else if (!ft_strcmp(cmd, "export"))
		export_builtin(argv, env);
	else if (!ft_strcmp(cmd, "unset"))
		unset_builtin(argv, env);
	else if (!ft_strcmp(cmd, "env"))
		env_builtin(env->exported);
	else if (!ft_strcmp(cmd, "exit"))
		exit_builtin(argv);
	else
		runsearch(cmd, argv, env->exported);
	free(argv);
}
