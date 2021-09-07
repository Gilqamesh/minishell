#include "../headers/ft_minishell.h"

char	*myreadline(const char *promptMsg)
{
	return (readline(promptMsg));
}

/*
** Checks if 'CmdLine' is properly nested or not
** Converts 'CmdLine' into a NULL terminated array of strings (tokens)
*/
void	parseCmdLine(t_minishell *mystruct, char *CmdLine)
{
	char	**tokens;

	if (isValidCmdLine(CmdLine) == false)
	{
	// Could just ignore the CmdLine and skip this case entirely (pdf)
	// Or give new prompt to the user waiting for the enclosing quotes (bash)
		printf("Not nested properly\n");
		return ;
	}
	tokens = convertStrToTokens(CmdLine);
	int i = -1;
	while (tokens[++i])
		ft_printf("%s\n", tokens[i]);
	ft_destroy_str_arr(&tokens);
	(void)mystruct;
}
