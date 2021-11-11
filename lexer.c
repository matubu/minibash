#include "minishell.h"

//TODO fix last empty string if end by space
static int	tokenize(char *s, void (*token)(char *s, int n, void *arg), void *arg)
{
	int	n;
	int	m;

	n = 0;
	while (1)
	{
		if (s[n] == '"' || s[n] == '\'')
		{
			m = n;
			while (s[++n] != s[m])
				if (s[n] == '\0')
					return (err("syntax error unclosed token", s));
			n++;
		}
		else if (s[n] != ' ')
			while (s[++n] && s[n] != ' ')
				;
		if (s[n] == ' ' || s[n] == '\0')
		{
			token(s, n, arg);
			if (s[n] == '\0')
				return (0);
			s += n + 1;
			n = 0;
		}
	}
}

static void	inc(char *s, int n, void *arg)
{
	(void)s;
	if (n)
		(*(int *)arg)++;
}

static char *token_substr(char *s, int n)
{
	char	*str;
	int		i;

	str = malloc((n + 1) * sizeof(char));
	i = 0;
	while (n-- > 0)
		if (*s == '\'' && s++)
			while (n-- && *s != '\'')
				str[i++] = *s++;
		else if (*s == '"' && s++)
			while (n-- && *s != '"')
				str[i++] = *s++;
		else
			str[i++] = *s++;
	str[i] = '\0';
	return (str);
}

#include <stdio.h>

static void	fill(char *s, int n, t_token *arg)
{
	if (!n)
		return ;
	printf("fill:%d:%.*s\n", n, n, s);
	while (arg->value)
		arg++;
	arg->expendable = *s != '\'';
	arg->value = token_substr(s, n);
	(++arg)->value = NULL;
}

t_token	*create_tokens(char *s)
{
	int		len;
	t_token	*tokens;

	len = 1;
	if (tokenize(s, inc, &len) == -1)
		return (NULL);
	tokens = malloc(len * sizeof(t_token));
	if (tokens == NULL)
		return (NULL);
	tokens->value = NULL;
	tokenize(s, (void (*)(char *, int, void *))fill, tokens);
	return (tokens);
}
