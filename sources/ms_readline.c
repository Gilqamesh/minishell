#include "../headers/ft_minishell.h"

char	*myreadline(const char *promptMsg)
{
	char	*buf;

	buf = readline(promptMsg);
	if (buf == NULL)
		return (NULL);
	if (ft_strlen(buf) > 0)
		add_history(buf);
	return (buf);
}

/*
** Parses the cmd line then executes the pipeline if its syntactically correct.
** Proper nesting:
** Could just ignore the CmdLine and skip this case entirely (pdf)
** Or give new prompt to the user waiting for the enclosing quotes (bash)
*/
int	parseCmdLine(t_minishell *mystruct)
{
	if (isProperlyNested(mystruct->promptStr) == false)
	{
		ft_putstr_fd("Not nested properly\n", STDERR_FILENO);
		return (1);
	}
	if (lexer(mystruct))
		return (1);
	// ft_printf("AFTER LEXER:\n");
	// ft_printf("Tokens START:\n");
	// int i = -1;
	// while (mystruct->tokens[++i])
	// 	ft_printf("|%s|\n", mystruct->tokens[i]);
	// ft_printf("END\n");
	if (expander(mystruct))
		return (1);
	// ft_printf("AFTER EXPANDER:\n");
	// ft_printf("Tokens START:\n");
	// i = -1;
	// while (mystruct->tokens[++i])
	// 	ft_printf("|%s|\n", mystruct->tokens[i]);
	// ft_printf("END\n");
	if (quoteRemoval(mystruct))
		return (1);
	// ft_printf("AFTER QUOTE REMOVAL:\n");
	// ft_printf("Tokens START:\n");
	// i = -1;
	// while (mystruct->tokens[++i])
	// 	ft_printf("|%s|\n", mystruct->tokens[i]);
	// ft_printf("END\n");
	if (checkSyntax(mystruct))
		return (1);
	if (parser(mystruct))
		return (1);
	if (executor(mystruct))
		return (1);
	clearStruct(mystruct);
	return (0);
}
