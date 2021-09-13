/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/13 14:34:44 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Allocates and returns a 't_shell_node*' with 'type' and 'data' initialized.
*/
static t_shell_node	*createNode(int type, void *data, t_std_FDs *FDs)
{
	t_shell_node	*newNode;

	newNode = ft_calloc(1, sizeof(*newNode));
	newNode->type = type;
	if (type == TYPE_OPERATOR)
		newNode->u_data.operator = data;
	else if (type == TYPE_SIMPLE_CMD)
	{
		newNode->u_data.simple_cmd.arguments = data;
		newNode->u_data.simple_cmd.FDs = *FDs;
	}
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
// int	parser(t_minishell *mystruct)
// {
// 	int				i;
// 	t_node_binary	*tmp;

// 	i = -1;
// 	while (mystruct->tokens[++i])
// 	{
// 		if (isValidOperator(mystruct->tokens[i]))
// 			ft_shell_node_add_back(&mystruct->nodes, createNode(
// 				TYPE_OPERATOR, ft_strdup(mystruct->tokens[i])));
// 		else if (isValidCommand(mystruct, mystruct->tokens[i]))
// 		{
// 			tmp = NULL;
// 			while (mystruct->tokens[i]
// 				&& isValidOperator(mystruct->tokens[i]) == NULL)
// 				ft_nodbinadd_front(&tmp, ft_nodbinnew(ft_strdup(
// 					mystruct->tokens[i++])));
// 			ft_shell_node_add_back(&mystruct->nodes, createNode(
// 				TYPE_COMMAND, ft_nodbinstr_to_strarr(tmp)));
// 			ft_nodbinclear(&tmp, ft_nodbindel, -1);
// 			i--;
// 		}
// 		else if (isValidFilename(mystruct->tokens[i]))
// 			ft_shell_node_add_back(&mystruct->nodes, createNode(
// 				TYPE_FILENAME, ft_strdup(mystruct->tokens[i])));
// 		else
// 		{
// 			ft_printf("%s: command not found\n", mystruct->tokens[i]);
// 			clearStruct(mystruct);
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

int	parser(t_minishell *mystruct)
{
	int				i;
	t_shell_node	*shell_node;
	t_node_binary	*tmp;
	char			*op;

	i = 0;
	while (mystruct->tokens[i])
	{
		if (isValidOperator(mystruct->tokens[i]) == NULL)
		{
			ft_printf("parse error near %s\n", mystruct->tokens[i]);
			clearStruct(mystruct);
			return (1);
		}
		ft_shell_node_add_back(&mystruct->nodes, createNode(TYPE_OPERATOR,
			ft_strdup(mystruct->tokens[i++]), NULL));
		tmp = NULL;
		while (mystruct->tokens[i]
			&& isValidOperator(mystruct->tokens[i]) == NULL)
			ft_nodbinadd_front(&tmp, ft_nodbinnew(ft_strdup(
				mystruct->tokens[i++])));
		shell_node = malloc(sizeof(*shell_node));
		shell_node->type = TYPE_SIMPLE_CMD;
		shell_node->u_data.simple_cmd.arguments = ft_nodbinstr_to_strarr(tmp);
		while (mystruct->tokens[i] && ft_strcmp(mystruct->tokens[i], "|"))
		{
			op = isValidOperator(mystruct->tokens[i]);
			if (!ft_strcmp(op, "<<"))
				ft_filelstadd_back(&shell_node->u_data.simple_cmd.FDs.inFiles,
					ft_filelstnew(ft_strdup(mystruct->tokens[++i]),
					REDIR_HEREDOC));
			else if (!ft_strcmp(op, ">>"))
				ft_filelstadd_back(&shell_node->u_data.simple_cmd.FDs.inFiles,
					ft_filelstnew(ft_strdup(mystruct->tokens[++i]),
					REDIR_APPEND));
			else if (!ft_strcmp(op, "<"))
				ft_filelstadd_back(&shell_node->u_data.simple_cmd.FDs.inFiles,
					ft_filelstnew(ft_strdup(mystruct->tokens[++i]),
					REDIR_IN));
			else if (!ft_strcmp(op, ">"))
				ft_filelstadd_back(&shell_node->u_data.simple_cmd.FDs.inFiles,
					ft_filelstnew(ft_strdup(mystruct->tokens[++i]),
					REDIR_OUT));
			else
			{
				ft_printf("The operation %s is not supported currently\n",
					mystruct->tokens[++i]);
				clearStruct(mystruct);
				return (1);
			}
			
		}
	}
	return (0);
}