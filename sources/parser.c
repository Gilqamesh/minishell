/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/10 11:49:54 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Allocates and returns a 't_shell_node*' with 'type' and 'data' initialized.
*/
static t_shell_node	*createNode(int type, void *data)
{
	t_shell_node	*newNode;

	newNode = ft_calloc(1, sizeof(*newNode));
	newNode->type = type;
	if (type == TYPE_OPERATOR)
		newNode->u_data.operator = data;
	else if (type == TYPE_FILENAME)
		newNode->u_data.filename = data;
	else if (type == TYPE_COMMAND)
		newNode->u_data.arguments = data;
	return (newNode);
}

/*
** Parses the tokens and builds the 'nodes' list in 'mystruct' using 't_node'
** Each token check if:
** - It is a valid operator or
** - A valid command or
** - A valid filename
** If none of the above, then we have an invalid token.
*/
int	parser(t_minishell *mystruct)
{
	int				i;
	t_node_binary	*tmp;

	i = -1;
	while (mystruct->tokens[++i])
	{
		if (isValidOperator(mystruct->tokens[i]))
			ft_shell_node_add_back(&mystruct->nodes, createNode(
				TYPE_OPERATOR, ft_strdup(mystruct->tokens[i])));
		else if (isValidCommand(mystruct, mystruct->tokens[i]))
		{
			tmp = NULL;
			while (mystruct->tokens[i] && !isValidOperator(mystruct->tokens[i]))
				ft_nodbinadd_front(&tmp, ft_nodbinnew(ft_strdup(
					mystruct->tokens[i++])));
			ft_shell_node_add_back(&mystruct->nodes, createNode(
				TYPE_COMMAND, ft_nodbinstr_to_strarr(tmp)));
			ft_nodbinclear(&tmp, ft_nodbindel, -1);
			i--;
		}
		else if (isValidFilename(mystruct->tokens[i]))
			ft_shell_node_add_back(&mystruct->nodes, createNode(
				TYPE_FILENAME, ft_strdup(mystruct->tokens[i])));
		else
		{
			ft_printf("%s: command not found\n", mystruct->tokens[i]);
			free_mystruct(mystruct);
			return (1);
		}
	}
	return (0);
}
