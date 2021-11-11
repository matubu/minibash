#include "minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	size_t	j;

	i = ft_strlen(dest);
	j = 0;
	while (src[j] != 0)
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = 0;
	return (dest);
}

unsigned int	ft_strlcpy(char *dst, const char *src, unsigned int size)
{
	unsigned int	i;

	if (size == 0)
		return (ft_strlen(src));
	i = 0;
	while (--size && *src && ++i)
		*dst++ = *src++;
	*dst = '\0';
	while (*src++)
		i++;
	return (i);
}

int	ft_strisonly(const char *s, char c)
{
	while (*s)
		if (*s++ != c)
			return (0);
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 || *s2))
		if (*s1++ != *s2++)
			return (*(unsigned char *)--s1 - *(unsigned char *)--s2);
	return (0);
}
