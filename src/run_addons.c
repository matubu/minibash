/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_addons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 17:02:13 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/24 17:04:33 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* will concatenate a path with a relative path
* @param {char *} path the absolute path
* @param {int} the path string length
* @param {char *} the path relative to path ex: cat
*/
char	*pathncat(char *path, int n, char *relative)
{
	char	*full;
	char	*start;

	full = malloc(n + ft_strlen(relative) + 2);
	if (full == NULL)
		return (NULL);
	start = full;
	while (n--)
		*full++ = *path++;
	*full++ = '/';
	while (*relative)
		*full++ = *relative++;
	*full = '\0';
	return (start);
}
