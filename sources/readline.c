#include "../headers/ft_minishell.h"

char	*myreadline(const char *promptMsg)
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
	int i = -1;
	while (tokens[++i])
		ft_printf("%s\n", tokens[i]);
	ft_destroy_str_arr(&tokens);
	(void)mystruct;
}

/*
** Allocates and returns a NULL terminated array of strings.
** The strings are separated by metacharacters.
*/
char	**convertStrToTokens(char *str)
{
	return (ft_splitstr(str, " \t\n|&;()<>"));
}
