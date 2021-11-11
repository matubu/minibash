#include "minishell.h"

//TODO maybe use isspace istead of *s == ' '
static int	tokenize(char *s, void (*token)(char *s, int n, void *arg), void *arg)
{
	int	n;

	n = 0;
	while (*s)
	{
		if (*s == ' ' || *s == '"' || *s == '\'')
		{
			token(s - n, n, arg);
			n = 0;
		}
		if (*s == '"' || *s == '\'')
		{
			while (s[++n] && s[n] != *s)
				;
			if (s[n] != *s)
				return (-1);
			token(s, n + 1, arg);
			s += n + 1;
			n = 0;
		}
		else if (*s == ' ')
			while (*++s == ' ')
				;
		else
		{
			n++;
			s++;
		}
	}
	token(s - n, n, arg);
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
	{
		err("not terminated string", s);
		return (NULL);
	}
	tokens = malloc(len * sizeof(char *));
	if (tokens == NULL)
		return (NULL);
	*tokens = NULL;
	tokenize(s, (void (*)(char *, int, void *))fill, tokens);
	return (tokens);
}
