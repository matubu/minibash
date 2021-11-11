#include "minishell.h"

int	g_process = 0;

/**
* @param {Bool} b if true (1) the terminal will show ^C or ^\ else this will hide them
*/
void	show_ctl(int b)
{
	struct termios new;

	tcgetattr(0, &new);
	if (b)
		new.c_lflag |= ECHOCTL;
	else
		new.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &new);
}

void	handle_sigquit(int signum)
{
	(void)signum;
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigint(int signum)
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
//TODO ctrl \ on cat
int	main(int argc, char **argv, char **env)
{
	char	*line;
	char	**cargv;

	(void)argc;
	(void)argv;
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		show_ctl(0);
		line = readline(PS1);
		if (line == NULL)
			return (0);
		add_history(line);
		cargv = ft_split(line, ' ');
		if (cargv == NULL)
			return (1);
		show_ctl(1);
		if (*cargv)
			run(*cargv, cargv, env);
		ft_free_splits(cargv);
		free(line);
	}
	return (0);
}
