/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:39 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/25 11:59:29 by acoezard         ###   ########.fr       */
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
	int	len;

	len = 0;
	while (s[len])
		len++;
	write(fd, s, len);
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
int	error(char *name, char *err, char *info, int code)
{
	putstr(2, name);
	write(2, ": ", 2);
	putstr(2, err);
	write(2, ": ", 2);
	putstr(2, info);
	write(2, "\n", 1);
	g_process.code = code;
	return (-1);
}

/**
* function that write minishell error in a coherent way
* minishell: error: file
*/
int	err(char *err, char *info, int code)
{
	return (error(NAME, err, info, code));
}
