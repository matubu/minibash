/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 16:23:43 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/08 17:13:41 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO support path like ~/test
//TODO test///test
void	cd(char **argv)
{
	if (argv[0] && argv[1])
	{
		if (chdir(argv[1]) == -1)
			err(strerror(errno), argv[1]);
	}
	else if (chdir(getenv("HOME")) == -1)
		err(strerror(errno), getenv("HOME"));
}

void	echo(char **argv)
{
	int	nl;

	nl = 1;
	if (!ft_strcmp(*argv, "-n") && argv++)
		nl = 0;
	while (*argv)
	{
		putstr(1, *argv);
		if (*++argv)
			write(1, " ", 1);
	}
	if (nl)
		write(1, "\n", 1);
}

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
