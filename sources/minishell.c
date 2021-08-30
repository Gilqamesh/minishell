#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char **argv, char **envp)
{
	char	*str;

	str = 
	while (1)
	{
		str = readline("Command: ");
		printf("readline return: %s\n", str);
		free(str);
	}
	exit(0);
}
