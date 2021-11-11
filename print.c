/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:39 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/08 18:09:45 by mberger-         ###   ########.fr       */
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

/**
* write a signed int to fd
* @param {int} fd
* @param {int} n
*/
void	putint(int fd, int n)
{
	if (n >= 0)
		n = -n;
	else
		write(fd, "-", 1);
	if (n <= -10)
		putint(fd, n / 10);
	n = '0' - n % 10;
	write(fd, &n, 1);
}

/**
* function that write error in a coherent way
* name: error: file
*/
int	error(char *name, char *err, char *info)
{
	putstr(2, name);
	write(2, ": ", 2);
	putstr(2, err);
	write(2, ": ", 2);
	putstr(2, info);
	write(2, "\n", 1);
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
