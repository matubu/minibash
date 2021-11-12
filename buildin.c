/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:23:43 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/12 11:32:55 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @param {char **} argv including the command: "cd"
* will change the cwd
*/
void	cd_buildin(char **argv)
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
void	echo_buildin(char **args)
{
	int	nl;

	nl = 1;
	if (*args && **args == '-' && ft_strisonly(*args + 1, 'n') && args++)
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
void	pwd_buildin(char **argv)
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

void	env_buildin(char **env)
{
	while (*env)
		println(1, *env++);
}

void	exit_buildin(char **argv)
{
	int		v;
	int		neg;
	char	*nptr;

	write(1, "exit\n", 5);
	if (argv[0] && argv[1] && argv[2])
		return ((void)err("exit", "too many arguments"));
	if (argv[0] && argv[1])
	{
		v = 0;
		nptr = argv[1];
		neg = *nptr == '-';
		while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
			nptr++;
		if (*nptr == '-' || *nptr == '+')
			nptr++;
		while (*nptr)
			if (*nptr >= '0' && *nptr <= '9')
				v = v * 10 - *nptr++ + '0';
		else if (error("bash: exit", argv[1], "numeric argument required"))
			exit(255);
		if (!neg)
			return (exit(-v));
		exit(v);
	}
	exit(0);
}
