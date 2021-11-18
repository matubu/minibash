/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:10:21 by acoezard          #+#    #+#             */
/*   Updated: 2021/11/18 15:26:26 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Format: [cmd et ses params] [< fichier_in] [> fichier_out ...]
// Si plusieurs fichier_in, garder seulement le dernier
// La redirection < met le stdin dans la cmd.
// La redirection > met le stdout dans le fichier_out.
// Le heredoc << indique le delimiteur a partir duquel la lecture du stdin doit s'arreter.
// => Prend seulement le dernier heredoc ou redirection < mais effectue quand meme la lecture du stdin.
// La redirection >> append le stdout dans le fichier_out.
