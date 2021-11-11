/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:23:43 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/09 14:08:19 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @param {char **} argv including the command: "cd"
* will change the cwd
*/
void	cd(char **argv)
{
	if (argv[0] && argv[1])
	{
		if (chdir(argv[1]) == -1)
			error("cd", strerror(errno), argv[1]);
	}
	else if (chdir(getenv("HOME")) == -1)
		error("cd", strerror(errno), getenv("HOME"));
}

/**
* @param {char **} args not including the command: "echo"
* will parse the flags and write to the stdout the other arguments
*/
void	echo(char **args)
{
	int	nl;

	nl = 1;
	if (*args && !ft_strcmp(*args, "-n") && args++)
		nl = 0;
	while (*args)
	{
		putstr(1, *args);
		if (*++args)
			write(1, " ", 1);
	}
	if (nl)
		write(1, "\n", 1);
}

/**
* @parms argv including pwd
* will write the cwd to the stdout except if their is two arguments
*/
void	pwd(char **argv)
{
	char	path[PATH_BUF];

	if (argv[0] && argv[1])
		putstr(2, "usage: pwd\n");
	else
	{
		if (getcwd(path, PATH_BUF) == NULL)
		{
			if (errno == ERANGE)
				putstr(2, "pwd: pathname length exceeds the buffer size\n");
		}
		else
			println(1, path);
	}
}
