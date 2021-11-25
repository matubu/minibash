/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 17:45:16 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/25 11:26:32 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\f'));
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\v' \
		|| c == '\n' || c == '\f' || c == '\r');
}

int	is_valid_long(const char *nptr, long *v)
{
	int    neg;

    *v = 0;
    while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
        nptr++;
    neg = *nptr == '-';
    if (*nptr == '-' || *nptr == '+')
        nptr++;
    if (!*nptr)
        return (0);
    while (*nptr >= '0' && *nptr <= '9')
        if (*v * 10 - *nptr + '0' > *v)
            return (0);
    else
        *v = *v * 10 - *nptr++ + '0';
    if (!neg && *v < *v - 1)
        return (0);
    if (!neg)
        *v = -*v;
    if (*nptr)
        return (0);
    return (1);
}
