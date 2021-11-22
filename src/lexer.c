/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 08:37:50 by mberger-          #+#    #+#             */
/*   Updated: 2021/11/22 11:01:29 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	inc(char *s, int n, void *arg)
{
	(void)s;
	if (n > 0)
		(*(int *)arg)++;
	return (0);
}

static void	sub_tokenize(char *s, int n, void (*sub)(char *s, int n, void *arg),
		void *arg)
{
	int	len;

	while (n > 0)
	{
		if (*s == '"' || *s == '\'')
		{
			len = 0;
			while (n-- > 0 && s[++len] != *s)
				;
			sub(s++, len - 1, arg);
			s += len;
			n--;
		}
		else
		{
			len = 0;
			while (s[len] != '"' && s[len] != '\'' && n-- > 0)
				len++;
			sub(s, len, arg);
			s += len;
		}
	}
}

static void	sub_fill(char *s, int n, t_token *arg)
{
	if (n <= 0)
		return ;
	while (arg->value)
		arg++;
	arg->expendable = *s;
	if (*s == '\'' || *s == '"')
		s++;
	arg->value = malloc((n + 1) * sizeof(char));
	arg->value[n] = '\0';
	while (n--)
		arg->value[n] = s[n];
	(++arg)->value = NULL;
}

static int	fill(char *s, int n, t_token **arg)
{
	int	len;

	if (n <= 0)
		return (0);
	while (*arg)
		arg++;
	len = 1;
	sub_tokenize(s, n, (void (*)()) inc, &len);
	*arg = malloc(len * sizeof(t_token));
	if (*arg == NULL)
		return (0);
	(*arg)->value = NULL;
	sub_tokenize(s, n, (void (*)()) sub_fill, *arg);
	*++arg = NULL;
	return (0);
}

t_token	**create_tokens(char *s)
{
	int		len;
	t_token	**tokens;

	len = 1;
	if (tokenize(s, inc, &len) == -1)
		return (NULL);
	tokens = malloc(len * sizeof(t_token *));
	if (tokens == NULL)
		return (NULL);
	*tokens = NULL;
	tokenize(s, (int (*)()) fill, tokens);
	return (tokens);
}
