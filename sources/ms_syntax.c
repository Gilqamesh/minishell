/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 13:49:24 by edavid            #+#    #+#             */
/*   Updated: 2021/09/22 16:46:24 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static int	parse_filename(char *str)
{
	if (isValidFilename(str) == true)
		return (0);
	return (1);
}

/*
** <command			::=	<simple command>
** 	        		|	<simple command> '<' <filename>
** 	        		|	<simple command> '>' <filename>
*/
static int	parse_simpleCmd(t_minishell *mystruct, int index)
{
	if (mystruct->tokens[index] == NULL)
		return (1);
	while (mystruct->tokens[index] && ft_strcmp(mystruct->tokens[index], "|"))
	{
		if (isValidOperator(mystruct->tokens[index])
			&& parse_filename(mystruct->tokens[++index]))
		{
			ft_putstr_fd("syntax error near unexpected token `",
				STDERR_FILENO);
			if (mystruct->tokens[index] == NULL)
				ft_putendl_fd("newline'", STDERR_FILENO);
			else
			{
				ft_putstr_fd(mystruct->tokens[index], STDERR_FILENO);
				ft_putendl_fd("'", STDERR_FILENO);
			}
			return (1);
		}
		index++;
	}
	return (0);
}

/*
** if (mystruct->tokens[start] == NULL)
** the original bash waits for a <command> to be entered.
** In this function it is not supported.
** <job>			::=	<command>
** 					|	< job > '|' < command >
*/
static int	parse_job(t_minishell *mystruct, int start)
{
	int	end;

	if (mystruct->tokens[start] == NULL
		|| !ft_strcmp(mystruct->tokens[start], "|"))
	{
		ft_printf("no <command> after pipe\n");
		return (1);
	}
	end = start;
	while (mystruct->tokens[end + 1]
		&& ft_strcmp(mystruct->tokens[end + 1], "|"))
		end++;
	if (mystruct->tokens[end + 1] && parse_job(mystruct, end + 2))
		return (1);
	if (parse_simpleCmd(mystruct, start))
		return (1);
	return (0);
}

/*
** Checks for syntax before parsing.
** Does not support: '&', ';'
** The shell language grammer is defined as follows in Backus–Naur form:
** <command line>	::	<job>					 <- ATM ONLY THIS IS SUPPORTED
** 					|	<job> '&'
** 					| 	<job> '&' <command line>
** 					|	<job> ';'
** 					|	<job> ';' <command line>
*/
int	checkSyntax(t_minishell *mystruct)
{
	if (mystruct->tokens[0] == NULL)
		return (0);
	if (parse_job(mystruct, 0))
	{
		clearStruct(mystruct);
		return (1);
	}
	return (0);
}
