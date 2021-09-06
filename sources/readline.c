#include "../headers/ft_minishell.h"

char	*myreadline(const char *promptMsg)
{
	return (readline(promptMsg));
}

/*
** Returns true if 'str' is obeying the quoting rules in bash, false otherwise
*/
bool	isProperlyNested(char *str)
{
	if (str == NULL)
		return (true);
	while (*str)
	{
		if (*str == '\'')
			str = ft_strchr(str + 1, '\'');
		else if (*str == '\"')
			str = ft_strchr(str + 1, '\"');
		if (str == NULL)
			return (false);
		str++;
	}
	return (true);
}

/*
** Checks if 'CmdLine' is quoted properly
** And that metacharacters are used properly
*/
bool	isValidCmdLine(char *CmdLine)
{
	return (isProperlyNested(CmdLine));
}

/*
** Checks if 'CmdLine' is properly nested or not
** Converts 'CmdLine' into a NULL terminated array of strings (tokens)
*/
void	parseCmdLine(t_minishell *mystruct, char *CmdLine)
{
	char	**tokens;

	if (isValidCmdLine(CmdLine) == false)
	// Could just ignore the CmdLine and skip this case entirely (pdf)
	// Or give new prompt to the user waiting for the enclosing quotes (bash)
		printf("Not nested properly\n");
	tokens = convertStrToTokens(CmdLine);
	ft_destroy_str_arr(&tokens);
}

/*
** Allocates and returns a NULL terminated array of strings.
** The strings are separated by metacharacters.
*/
char	**convertStrToTokens(char *str)
{
	return (ft_splitstr(str, " \t\n|&;()<>"));
}
