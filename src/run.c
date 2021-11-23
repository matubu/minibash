/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:38 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/23 17:05:37 by mberger-         ###   ########.fr       */
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
	if (access(cmd, X_OK) == -1)
		return (-1);
	printf("found %s\n", cmd);
	execve(cmd, argv, env);
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

	printf("cmd %s\n", cmd);
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

char	**create_argv(char *cmd, t_env *env)
{
	char	**argv;
	t_token	**tokens;

	tokens = create_tokens(cmd);
	if (tokens == NULL || *tokens == NULL)
	{
		free_tokens(tokens);
		err("command not found", "(null)");
		return (NULL);
	}
	printf("--->%s\n", tokens[0]->value);
	env_expand(env->local, tokens);
	printf("--->%s\n", tokens[0]->value);
	wildcard_expand(&tokens);
	if (tokens[0]->value == NULL)
	{
		free_tokens(tokens);
		err("command not found", "(null)");
		return (NULL);
	}
	argv = token_to_argv(tokens);
	free_tokens(tokens);
	return (argv);
}

int	exec_builtin(char *cmd, t_env *env, int stdout)
{
	char	**argv;

	argv = create_argv(cmd, env);
	if (argv == NULL)
		return (0);
	if (isenvdefine(*argv))
		set_builtin(argv, env);
	else if (!ft_strcmp(*argv, "echo"))
		echo_builtin(stdout, argv + 1);
	else if (!ft_strcmp(*argv, "cd"))
		cd_builtin(env, argv + 1);
	else if (!ft_strcmp(*argv, "pwd"))
		pwd_builtin(stdout, argv + 1);
	else if (!ft_strcmp(*argv, "export"))
		export_builtin(argv + 1, env);
	else if (!ft_strcmp(*argv, "unset"))
		unset_builtin(argv + 1, env);
	else if (!ft_strcmp(*argv, "env"))
		env_builtin(stdout, env->exported);
	else if (!ft_strcmp(*argv, "exit"))
		exit_builtin(stdout, argv + 1);
	else
		return (free_argv(argv));
	free_argv(argv);
	return (1);
}
