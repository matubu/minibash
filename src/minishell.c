/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:44 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/17 16:57:56 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_process	g_process = {0, 0};

/**
* @param	{Bool}	b	if true (1) the terminal will show ^C or ^\ else
						this will hide them.
*/
void	show_ctl(int b)
{
	struct termios	new;

	tcgetattr(0, &new);
	if (b)
		new.c_lflag |= ECHOCTL;
	else
		new.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &new);
}

static void	handle_sigquit(int signum)
{
	(void)signum;
	if (g_process.pid)
	{
		if (kill(g_process.pid, SIGQUIT) == 0)
			putstr(1, "Quit: 3\n");
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
	}
	g_process.pid = 0;
}

static void	handle_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	if (g_process.pid)
		kill(g_process.pid, SIGINT);
	else
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_process.pid = 0;
}

// TODO: #14 value=test echo helloworld => will only display helloworld
// TODO: #16 buildin return value + parsing error
// TODO: #19 export $?
int	main(int argc, char **argv, char **envm)
{
	char			*line;
	static t_env	env = {NULL, NULL};

	(void)argc;
	(void)argv;
	env_init(&env, envm);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		show_ctl(0);
		line = readline(PS1);
		if (line == NULL)
			break ;
		add_history(line);
		//pipe_execute(&env, line);
		pipe_parse(&env, line);
		free(line);
	}
	write(1, "exit\n", 5);
	exit(EXIT_SUCCESS);
	return (0);
}
