/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberger- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 17:55:19 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/07 18:28:56 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	main(int argc, char **argv, char **env)
{
	char	*args;

	(void)argc;
	(void)argv;
	args = NULL;
	*env = "PS1=minishell$ ";
	if (execve("/bin/bash", &args, env) == -1)
		return (1);
	return (0);
}
