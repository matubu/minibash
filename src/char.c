/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 17:22:00 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/25 15:25:16 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*char_cat(char *dest, const char src)
{
	size_t	i;

	i = ft_strlen(dest);
	dest[i] = src;
	dest[i + 1] = 0;
	return (dest);
}

char	*char_join(const char *s1, const char c)
{
	size_t	s1_len;
	char	*sptr;

	if (!s1 || !c)
		return (NULL);
	s1_len = ft_strlen(s1);
	sptr = (char *) malloc(s1_len + 2);
	if (!sptr)
		return (NULL);
	ft_strcpy(sptr, s1);
	char_cat(sptr, c);
	return (sptr);
}
