#include "minishell.h"

//TODO fix: ./test.sh should not throw an error
int	main(int argc, char **argv, char **env)
{
	char	*line;
	char	**cargv;

	(void)argc;
	(void)argv;
	while (1)
	{
		line = readline(PS1);
		if (line == NULL)
			return (0);
		//add_history(line);
		cargv = ft_split(line, ' ');
		if (cargv == NULL)
			return (1);
		run(*cargv, cargv, env);
		add_history(line);
		free(line);
	}
	return (0);
}
