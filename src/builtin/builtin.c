/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:23:43 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/24 13:07:08 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @param {char **} argv including the command: "cd"
* will change the cwd
*/
void	cd_builtin(t_env *env, char **argv)
{
	char	*pwd;
	char	buf[PATH_BUF];
	char	*status;

	if (*argv)
		status = *argv;
	else
		status = getenv("HOME");
	if (chdir(status) == -1)
		error("cd", strerror(errno), status);
	else
	{
		pwd = ft_strjoin("OLD_", *env_get(env->exported, "PWD"));
		env_set(&env->local, pwd);
		env_set(&env->exported, pwd);
		free(pwd);
		pwd = ft_strjoin("PWD=", getcwd(buf, PATH_BUF));
		env_set(&env->local, pwd);
		env_set(&env->exported, pwd);
		free(pwd);
		g_process.code = 0;
	}
}

/**
* @param {char **} args not including the command: "echo"
* will parse the flags and write to the stdout the other arguments
*/
void	echo_builtin(int stdout, char **args)
{
	int	nl;

	nl = 1;
	while (*args && **args == '-' && ft_strisonly(*args + 1, 'n') && args++)
		nl = 0;
	while (*args && **args == '\0')
		args++;
	while (*args)
	{
		putstr(stdout, *args);
		if (*++args)
			write(stdout, " ", 1);
	}
	if (nl)
		write(stdout, "\n", 1);
	g_process.code = 0;
}

/**
* @parms argv including pwd
* will write the cwd to the stdout except if their is two arguments
*/
void	pwd_builtin(int stdout, char **argv)
{
	char	path[PATH_BUF];

	if (*argv)
		err("usage", "pwd");
	else
	{
		if (getcwd(path, PATH_BUF) == NULL)
		{
			if (errno == ERANGE)
				return ((void)err("pwd",
						"pathname length exceeds the buffer size"));
		}
		else
			println(stdout, path);
		g_process.code = 0;
	}
}

void	env_builtin(int stdout, char **env)
{
	while (*env)
		if (**env)
			println(stdout, *env++);
	else
		env++;
	g_process.code = 0;
}

void	exit_builtin(int stdout, char **argv)
{
	int		v;
	int		neg;
	char	*nptr;

	g_process.code = 0;
	write(stdout, "exit\n", 5);
	if (*argv && argv[1])
		return ((void)err("exit", "too many arguments"));
	if (!*argv)
		exit(0);
	v = 0;
	nptr = *argv;
	while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
		nptr++;
	neg = *nptr == '-';
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	while (*nptr)
		if (*nptr >= '0' && *nptr <= '9')
			v = v * 10 - *nptr++ + '0';
	else if (error("bash: exit", argv[0], "numeric argument required"))
		exit(255);
	if (!neg)
		return (exit(-v));
	exit(v);
}
