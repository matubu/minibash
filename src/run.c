/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:38 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/18 14:50:59 by mberger-         ###   ########.fr       */
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
	//pid_t	pid;

	if (access(cmd, X_OK) == -1)
		return (-1);
	//pid = fork();
	//if (pid == 0)
	//{
		execve(cmd, argv, env);
	//	exit(-1);
	//}
	//else
	//{
	//	g_process.pid = pid;
	//	wait(&g_process.code);
	//}
	return (0);
}

/**
* will search in PATH folders the command if not a relative or absolute path
*/
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
}

int	exec_tokens(char *cmd, t_env *env)
{
	char	**argv;
	t_token	**tokens;

	tokens = create_tokens(cmd);
	env_expand(env->local, tokens);
	wildcard_expand(&tokens);
	if (tokens[0]->value == NULL)
		return (g_process.code = 1);
	argv = token_to_argv(tokens);
	free_tokens(tokens);
	run(argv[0], argv, env);
	free_argv(argv);
	return (0);
}

