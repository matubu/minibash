/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:39 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/08 16:57:37 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	putstr(int fd, char *s)
{
	while (*s)
		write(fd, s++, 1);
}

void	println(int fd, char *s)
{
	putstr(fd, s);
	write(fd, "\n", 1);
}

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

int	err(char *err, char *info)
{
	putstr(2, NAME);
	write(2, ": ", 2);
	putstr(2, err);
	write(2, ": ", 2);
	putstr(2, info);
	write(2, "\n", 1);
	return (-1);
}
