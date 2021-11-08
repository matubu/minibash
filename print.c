/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:42:39 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/08 10:46:25 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	putstr(int fd, char *s)
{
	while (*s)
		write(fd, s++, 1);
}

void	putchar(int fd, char c)
{
	write(fd, &c, 1);
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

static void	handle(int fd, char c, va_list args)
{
	if (c == 's')
		putstr(fd, va_arg(args, char *)); 
	else if (c == 'c')
		putchar(fd, va_arg(args, int)); 
	else if (c == 'd' || c == 'i')
		putint(fd, va_arg(args, int)); 
	else
		write(fd, &c, 1);
}

void	mprintf(int fd, char *s, ...)
{
	va_list	args;

	va_start(args, s);
	while (*s)
		if (*s == '%' && s++)
			handle(fd, *s++, args);
		else
			write(fd, s++, 1);
	va_end(args);
}
