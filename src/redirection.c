/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:10:21 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/18 17:48:16 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Format: [cmd et ses params] [< fichier_in] [> fichier_out ...]
// Si plusieurs fichier_in, garder seulement le dernier
// La redirection < met le stdin dans la cmd.
// La redirection > met le stdout dans le fichier_out.
// Le heredoc << indique le delimiteur a partir duquel la lecture du stdin
// doit s'arreter.
// => Prend seulement le dernier heredoc ou redirection < mais effectue
// quand meme la lecture du stdin.
// La redirection >> append le stdout dans le fichier_out.

#include "minishell.h"

#define	R_LEFT	0
#define	R_RIGHT	1

struct s_redirection
{
	int		type;
	char	*arg;
}
typedef struct s_redirection	t_redirection;

int		r_is_char(char c)
{
	return (c == '<' || c == '>');
}

int		r_code(char c)
{
	if (c == '<')
		return (R_LEFT);
	if (c == '>')
		return (R_RIGHT);
	return (-1);
}

void	r_parse(t_env *env, char *cmd)
{
	size_t	i;
	size_t	j;
	size_t	count;


	i = -1;
	while (cmd[++i] && found != 3)
	{
		if (cmd[i] != '<' && cmd[i] != '>')
			continue;
		else
		{
			j = r_search(env, cmd);
		}
	}

}
