#include "minishell.h"

int	g_process = 0;

void	nothing(int signum)
{
	(void)signum;
	rl_on_new_line();
	rl_redisplay();
}

void	kill_running(int signum)
{
	(void)signum;
	if (g_process)
		kill(g_process, SIGINT);
	else
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_process = 0;
}

//TODO echo a"b"c -> abc
//TODO echo -nnn test -> test
//TODO ctrl \ on cat
int	main(int argc, char **argv, char **env)
{
	char	*line;
	char	**cargv;

	(void)argc;
	(void)argv;
	signal(SIGQUIT, nothing);
	signal(SIGINT, kill_running);
	while (1)
	{
		line = readline(PS1);
		if (line == NULL)
			return (0);
		add_history(line);
		cargv = ft_split(line, ' ');
		if (cargv == NULL)
			return (1);
		if (*cargv)
			run(*cargv, cargv, env);
		ft_free_splits(cargv);
		free(line);
	}
	return (0);
}
