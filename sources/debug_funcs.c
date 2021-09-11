/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 19:42:41 by edavid            #+#    #+#             */
/*   Updated: 2021/09/11 16:31:41 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

void	printTokens(t_minishell *mystruct)
{
	int i;

	i = -1;
	ft_printf("Tokens:\n");
	while (mystruct->tokens[++i])
		ft_printf("%s\n", mystruct->tokens[i]);
}

void	printStrArr(char **strArr)
{
	if (strArr == NULL)
		return ;
	ft_printf("command: ");
	while (*strArr)
		ft_printf("%s ", *strArr++);
	ft_printf("\n");
}

void	printNodes(t_minishell *mystruct)
{
	t_shell_node	*tmp;

	tmp = mystruct->nodes;
	ft_printf("Nodes:\n");
	while (tmp)
	{
		if (tmp->type == TYPE_OPERATOR)
			ft_printf("operator: %s\n", tmp->u_data.operator);
		else if (tmp->type == TYPE_FILENAME)
			ft_printf("filename: %s\n", tmp->u_data.filename);
		else if (tmp->type == TYPE_COMMAND)
			printStrArr(tmp->u_data.arguments);
		else
			ft_printf("Wrong type for node in printNodes\n");
		tmp = tmp->next;
	}	
}
