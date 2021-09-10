#include "../headers/ft_minishell.h"

char	*myreadline(const char *promptMsg)
{
	return (readline(promptMsg));
}

/*
** Checks if 'CmdLine' is properly nested or not
** Converts 'CmdLine' into a NULL terminated array of strings (tokens)
** Builds 'CommandTable' in 'mystruct' based on the tokens
*/
int	parseCmdLine(t_minishell *mystruct)
{
	if (isValidCmdLine(mystruct->promptStr) == false)
	{
	// Could just ignore the CmdLine and skip this case entirely (pdf)
	// Or give new prompt to the user waiting for the enclosing quotes (bash)
		printf("Not nested properly\n");
		return (1);
	}
	if (lexer(mystruct))
		return (1);
	printTokens(mystruct);
	if (expander(mystruct))
		return (1);
	if (parser(mystruct))
		return (1);
	printNodes(mystruct);
	clearStruct(mystruct);
	return (0);
}
