/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:10:31 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/23 15:51:47 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(const char *str, int c)
{
	int		i;

	i = -1;
	while (str[++i] != (unsigned char) c)
		if (str[i] == '\0')
			return (NULL);
	return ((char *)str + i);
}

int	ft_strisonly(const char *s, char c)
{
	while (*s)
		if (*s++ != c)
			return (0);
	return (1);
}

char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*out;
	int		i;

	out = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (out == NULL)
		return (NULL);
	i = 0;
	while (*s1)
		out[i++] = *s1++;
	while (*s2)
		out[i++] = *s2++;
	out[i] = '\0';
	return (out);
}
