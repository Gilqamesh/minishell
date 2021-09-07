/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidCmdLine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:45:16 by edavid            #+#    #+#             */
/*   Updated: 2021/09/07 18:10:26 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

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
