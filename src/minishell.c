/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:44 by mberger-          #+#    #+#             */
/*   Updated: 2021/12/02 12:31:15 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_process	g_process = {0, 0, NULL};

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

static void	set_env_at_start(t_env *env)
{
	char	path[PATH_BUF];
	char	*tmp;

	if (getcwd(path, PATH_BUF) == NULL)
	{
		if (errno == ERANGE)
			err("bash", "pathname length exceeds the buffer size", 1);
		return ;
	}
	tmp = ft_strjoin("PWD=", path);
	env_set(&env->local, tmp);
	env_set(&env->exported, tmp);
	g_process.pwd = ft_strdup(path);
	free(tmp);
}

int	main(int argc, char **argv, char **envm)
{
	char			*line;
	static t_env	env = {NULL, NULL};

	(void)argc;
	(void)argv;
	env_init(&env, envm);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	set_env_at_start(&env);
	while (1)
	{
		g_process.pid = 0;
		show_ctl(0);
		line = readline(PS1);
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		show_ctl(1);
		if (!ft_strisonly(line, "\t\n\v\f\r "))
			orand(&env, line, 1, 0);
		free(line);
	}
	free(g_process.pwd);
	write(1, "exit\n", 5);
	return (0);
}
