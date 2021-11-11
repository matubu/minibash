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

//TODO ctrl \ on cat
//TODO value=test echo hellowolrd => will only display helloworld
//TODO =test echo hellowolrd => error
int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_token	*tokens;

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
		tokens = create_tokens(line);
		free(line);
		if (tokens == NULL)
			continue ;
		//TODO check before variable expansion if is a=b
		//TODO if first follow the pattern [a-zA-Z_]+=[^]* expend only after =
		env_expend(tokens);
		show_ctl(1);
		if (tokens->value)
			run(tokens->value, token_to_argv(tokens), &env);
		free_tokens(tokens);
	}
	return (0);
}
