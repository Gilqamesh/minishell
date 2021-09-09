/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 17:52:37 by edavid            #+#    #+#             */
/*   Updated: 2021/09/09 19:53:37 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Checks if 'str' is a valid operator.
** Currently supports: "<<", ">>", "<", ">", "|"
*/
bool	isValidOperator(char *str)
{
	if (!ft_strcmp(str, "<<") || !ft_strcmp(str, ">>"))
		return (true);
	if (ft_strlen(str) == 1)
		if (isCharPartOfSet(*str, OPERATORS))
			return (true);
	return (false);
}

/*
** Returns true if 'str' is a valid command.
*/
bool	isValidCommand(t_minishell *mystruct, char *str)
{
	static char	*builtIns[] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", NULL};
	char		*cmd;

	if (ft_isStrContainedInStrArr(str, builtIns))
		return (true);
	cmd = ft_strdup(str);
	cmd_path(&cmd, mystruct->envp);
	if (*cmd == '\0')
	{
		free(cmd);
		return (false);
	}
	free(cmd);
	return (true);
}

/*
** Returns true if 'str' is a valid filename.
** Does not look at its existence.
*/
bool	isValidFilename(char *str)
{
	if (ft_strlen(str) <= NAME_MAX)
		return (true);
	return (false);
}

/*
** Returns true if 'str' is obeying the quoting rules in bash, false otherwise
*/
static bool	isProperlyNested(char *str)
{
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
