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
		g_process = pid;
		wait(NULL);
	}
	return (0);
}

/**
* will search the command if not a relative or absulte path in the folders specified in then environment variable PATH
*/
//TODO permission denied error for command too ?
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
	path = getenv("PATH");
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

#include <stdio.h>

/**
* will execute custom function if one of the buildin
* else it will call runsearch
*/
//TODO return or set the exit code $?
void	run(char *cmd, char **argv, char ***env)
{
	if (argv == NULL)
		return ;
	if (!argv[1] && isenvdefine(cmd))
	{
		printf("envdefine\n");
		envdefine(env, cmd);
	}
	else if (!ft_strcmp(cmd, "echo"))
		echo_buildin(argv + 1);
	else if (!ft_strcmp(cmd, "cd"))
		cd_buildin(argv);
	else if (!ft_strcmp(cmd, "pwd"))
		pwd_buildin(argv);
	//TODO export
	//TODO unset
	else if (!ft_strcmp(cmd, "env"))
		env_buildin(*env);
	else if (!ft_strcmp(cmd, "exit"))
		exit(0);
	else
		runsearch(cmd, argv, *env);
	free(argv);
}
