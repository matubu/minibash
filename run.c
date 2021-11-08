#include "minishell.h"

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

int	runfrompath(char *cmd, char **argv, char **env)
{
	pid_t	pid;

	if (access(cmd, X_OK) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (err("fork error", cmd));
	if (pid == 0)
	{
		execve(cmd, argv, env);
		exit(-1);
	}
	else
		wait(NULL);
	return (0);
}

//TODO permission denied for command too ?
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

//TODO return or set the exit code $?
int	run(char *cmd, char **argv, char **env)
{
	if (!ft_strcmp(cmd, "echo"))
		echo(argv + 1);
	else if (!ft_strcmp(cmd, "cd"))
		cd(argv);
	else if (!ft_strcmp(cmd, "pwd"))
		pwd(argv);
	//TODO export
	//TODO unset
	else if (!ft_strcmp(cmd, "env"))
		while (*env)
			println(1, *env++);
	else if (!ft_strcmp(cmd, "exit"))
		exit(0);
	else
		return (runsearch(cmd, argv, env));
	return (0);
}
