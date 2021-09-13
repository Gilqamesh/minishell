/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 17:52:37 by edavid            #+#    #+#             */
/*   Updated: 2021/09/13 16:53:01 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Checks if 'str' is a valid operator.
** Currently supports: "<<", ">>", "<", ">", "|"
*/
char	*isValidOperator(char *str)
{
	if (str == NULL)
		return (NULL);
	if (!ft_strcmp(str, "<<"))
		return ("<<");
	if (!ft_strcmp(str, ">>"))
		return (">>");
	if (ft_strlen(str) == 1)
		if (isCharPartOfSet(*str, OPERATORS))
			return (str);
	return (NULL);
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
	cmd_path(&cmd, mystruct->envpLst);
	if (cmd == NULL)
		return (false);
	free(cmd);
	return (true);
}

/*
** Returns true if 'str' is a valid filename.
** Does not look at its existence.
*/
bool	isValidFilename(char *str)
{
	if (str == NULL)
		return (false);
	if (isValidOperator(str))
		return (false);
	if (!access(str, F_OK))
		return (true);
	if (ft_strlen(str) <= NAME_MAX)
		return (true);
	return (false);
}

/*
** Returns true if 'str' is obeying the quoting rules in bash, false otherwise
*/
bool	isProperlyNested(char *str)
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
** Returns 'str' if it is a valid redirection operator.
** Returns NULL if it is not.
*/
char	*isValidRedirection(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (NULL);
	return (isValidOperator(str));
}
