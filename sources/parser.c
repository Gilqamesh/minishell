/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/09 12:56:49 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Checks if 'token' is a valid operator.
** Currently supports: "<<", ">>", "<", ">", "|"
*/
static bool	isValidOperator(char *token)
{
	if (!ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"))
		return (true);
	if (ft_strlen(token) == 1)
		if (isCharPartOfSet(*token, OPERATORS))
			return (true);
	return (false);
}

/*
** Allocates and returns a 't_shell_node*' with 'type' and 'data'
*/
static t_shell_node	*createNode(int type, void *data)
{
	t_shell_node	*newNode;

	newNode = malloc(sizeof(*newNode));
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
*/
void	parser(t_minishell *mystruct, char **tokens)
{
	int				i;
	t_node_binary	*tmp;

	i = -1;
	while (tokens[++i])
	{
		if (isValidOperator(tokens[i]))
			ft_lstadd_back(&mystruct->nodes, ft_lstnew(createNode(
				TYPE_OPERATOR, ft_strdup(tokens[i]))));
		else if (isValidCommand(tokens[i]))
		{
			tmp = NULL;
			while (isValidOperator(tokens[i]) == false)
				ft_nodbinadd_front(&tmp, ft_nodbinnew(ft_strdup(tokens[i++])));
			ft_lstadd_back(&mystruct->nodes, ft_lstnew(createNode(
				TYPE_COMMAND, ft_nodbinstrarrjoin_from_back(tmp))));
			ft_nodbinclear(&tmp, ft_nodbindel, -1);
		}
		else if (isValidFilename(tokens[i]))
			ft_lstadd_back(&mystruct->nodes, ft_lstnew(createNode(
				TYPE_FILENAME, ft_strdup(tokens[i]))));
	}
}
