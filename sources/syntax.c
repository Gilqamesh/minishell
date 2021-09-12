// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   syntax.c                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2021/09/12 13:49:24 by edavid            #+#    #+#             */
// /*   Updated: 2021/09/12 15:11:47 by edavid           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** <simple command>::=	<pathname>
** 	        		|	<simple command>  <token>
*/
static int	parse_simple_cmd(t_minishell *mystruct, int start, int end)
{
	(void)start;
	(void)end;
	(void)mystruct;
	return (0);
}

static int	parse_filename(t_minishell *mystruct, int index)
{
	if (isValidFilename(mystruct->tokens[index]) == true)
		return (0);
	return (1);
}

/*
** <command			::=	<simple command>
** 	        		|	<simple command> '<' <filename>
** 	        		|	<simple command> '>' <filename>
*/
static int	parse_command(t_minishell *mystruct, int start, int end)
{
	int		tmp;
	bool	isSimpleCmd;

	if (mystruct->tokens[start] == NULL)
		return (0);
	if (isValidOperator(mystruct->tokens[start])
		&& (start || !ft_strcmp(mystruct->tokens[start], "|")))
	{
		ft_printf("parse error near `%s'\n", mystruct->tokens[start]);
		return (1);
	}
	isSimpleCmd = true;
	tmp = start;
	while (mystruct->tokens[++tmp])
	{
		if (isValidOperator(mystruct->tokens[tmp]))
		{
			isSimpleCmd = false;
			if (parse_filename(mystruct, tmp + 1))
			{
				ft_printf("parse error near `%s'\n", mystruct->tokens[tmp]);
				return (1);
			}
			if (parse_simple_cmd(mystruct, start, tmp - 1))
			{
				ft_printf("command not found: %s\n", mystruct->tokens[start]);
				return (1);
			}
		}
	}
	if (isSimpleCmd && parse_simple_cmd(mystruct, start, end))
	{
		ft_printf("command not found: %s\n", mystruct->tokens[start]);	
		return (1);
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
static int	parse_job(t_minishell *mystruct, int start, int end)
{
	if (mystruct->tokens[start] == NULL)
	{
		ft_printf("no <command> after pipe\n");
		return (1);
	}
	while (mystruct->tokens[end + 1]
		&& ft_strcmp(mystruct->tokens[end + 1], "|"))
		end++;
	if (mystruct->tokens[end + 1] && parse_job(mystruct, end + 2, end + 2))
		return (1);
	if (parse_command(mystruct, start, end))
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
	if (parse_job(mystruct, 0, 0))
	{
		clearStruct(mystruct);
		return (1);
	}
	return (0);
}
