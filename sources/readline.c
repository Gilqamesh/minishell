#include "../headers/ft_minishell.h"

char	*myreadline(const char *promptMsg)
{
	char	*buf;

	buf = readline(promptMsg);
	if (ft_strlen(buf) > 0)
		add_history(buf);
	return (buf);
}

/*
** Checks if 'CmdLine' is properly nested or not
** Converts 'CmdLine' into a NULL terminated array of strings (tokens)
** Builds 'CommandTable' in 'mystruct' based on the tokens
*/
int	parseCmdLine(t_minishell *mystruct)
{
	if (isProperlyNested(mystruct->promptStr) == false)
	{
	// Could just ignore the CmdLine and skip this case entirely (pdf)
	// Or give new prompt to the user waiting for the enclosing quotes (bash)
		ft_putstr_fd("Not nested properly\n", STDERR_FILENO);
		return (1);
	}
	if (lexer(mystruct))
		return (1);
	if (expander(mystruct))
		return (1);
	if (checkSyntax(mystruct))
		return (1);
	if (parser(mystruct))
		return (1);
	printNodes(mystruct);
	printPipelines(mystruct);
	if (executor(mystruct))
		return (1);
	clearStruct(mystruct);
	return (0);
}
