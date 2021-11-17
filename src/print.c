/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:39 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/17 13:57:01 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* simply write a string in fd
* @param {int} fd
* @param {char *} s
*/
void	putstr(int fd, char *s)
{
	while (*s)
		write(fd, s++, 1);
}

/**
* put a string to fd followed by a newline
* @param {int} fd
* @param {char *} s
*/
void	println(int fd, char *s)
{
	putstr(fd, s);
	write(fd, "\n", 1);
}

char	*itoa_buf(int n)
{
	int			i;
	static char	res[13];
	int			neg;

	i = 13;
	res[--i] = '\0';
	neg = 0;
	if (n < 0)
		neg = 1;
	else
		n = -n;
	while (1)
	{
		res[--i] = '0' - n % 10;
		n /= 10;
		if (!n)
			break ;
	}
	if (neg)
		res[--i] = '-';
	return (res + i);
}

/**
* function that write error in a coherent way
* name: error: file
*/
int	error(char *name, char *err, char *info)
{
	putstr(2, name);
	write(2, ": \033[31m", 7);
	putstr(2, err);
	write(2, "\033[0m: ", 6);
	putstr(2, info);
	write(2, "\n", 1);
	g_process.code = 1;
	return (-1);
}

/**
* function that write minishell error in a coherent way
* minishell: error: file
*/
int	err(char *err, char *info)
{
	return (error(NAME, err, info));
}
