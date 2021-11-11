#include "minishell.h"

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

static char	*substr(char *s, int n)
{
	char	*str;
	int		i;

	str = malloc((n + 1) * sizeof(char));
	i = 0;
	while (n--)
		if (*s != '"' && *s != '\'')
			str[i++] = *s++;
		else
			s++;
	str[i] = '\0';
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
