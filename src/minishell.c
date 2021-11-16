/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:44 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/16 13:27:55 by mberger-         ###   ########.fr       */
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

void	handle_sigquit(int signum)
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

void	handle_sigint(int signum)
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
// TODO: #15 =test echo helloworld => error
// TODO: #16 buildin return value + parsing error
// TODO: #17 a5=7
// TODO: #18 echo hello'$PATH'
// TODO: #19 export $?
int	main(int argc, char **argv, char **envm)
{
	char			*line;
	t_token			**tokens;
	static t_env	env = {NULL, NULL};

	while (*envm)
	{
		env_set(&env.exported, *envm);
		env_set(&env.local, *envm++);
	}
	(void)argc;
	(void)argv;
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		show_ctl(0);
		line = readline(PS1);
		if (line == NULL)
			break ;
		add_history(line);
		tokens = create_tokens(line);
		free(line);
		if (tokens == NULL)
			continue ;
		// TODO: #11 check before variable expansion if is a=b
		// TODO: #12 if first follow the pattern [a-zA-Z_]+=[^]* expend only after =
		//wildcard_expand(tokens);
		env_expand(env.local, tokens);
		show_ctl(1);
		//if (tokens[i]->value)
		run(tokens[0]->value, token_to_argv(tokens), &env);
		//else
		//	g_process.code = 0;
		free_tokens(tokens);
	}
	write(1, "exit\n", 5);
	return (0);
}
