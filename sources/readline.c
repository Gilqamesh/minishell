#include "../headers/ft_minishell.h"

char	*myreadline(t_minishell *mystruct, const char *promptMsg)
{
	return (readline(promptMsg));
}

/*
** 
*/
void	parseCmdLine(t_minishell *mystruct, char *CmdLine)
{
	char	**tokens;

	tokens = convertStrToTokens(CmdLine);
}

/*
** Allocates and returns a NULL terminated array of strings.
** The strings are separated by metacharacters.
*/
char	**convertStrToTokens(char *str)
{
	
}
