#include "minishell.h"

//TODO maybe use isspace istead of *s == ' '
static int	tokenize(char *s, void (*token)(char *s, int n, void *arg), void *arg)
{
	int	n;

	while (*s)
	{
		if (*s == '"' || *s == '\'')
		{
			n = 0;
			while (s[++n] != *s)
				if (s[n] == '\0')
					return (err("syntax error near unexpected token", s));
			token(s, n + 1, arg);
			s += n + 1;
		}
		else if (*s != ' ')
		{
			n = 1;
			while (*++s && *s != ' ' && *s != '"' && *s != '\'')
				n++;
			token(s - n, n, arg);
		}
		else
			s++;
	}
	return (0);
}

static void	inc(char *s, int n, void *arg)
{
	(void)s;
	if (n)
		(*(int *)arg)++;
}

static char	*substr(char *s, int n)
{
	char *str;

	str = malloc((n + 1) * sizeof(char));
	str[n] = '\0';
	while (n--)
		str[n] = s[n];
	return (str);
}

static void	fill(char *s, int n, char **arg)
{
	if (!n)
		return ;
	while (*arg)
		arg++;
	*arg++ = substr(s, n);
	*arg = NULL;
}

char	**create_tokens(char *s)
{
	int		len;
	char	**tokens;

	len = 1;
	if (tokenize(s, inc, &len) == -1)
		return (NULL);
	tokens = malloc(len * sizeof(char *));
	if (tokens == NULL)
		return (NULL);
	*tokens = NULL;
	tokenize(s, (void (*)(char *, int, void *))fill, tokens);
	return (tokens);
}
