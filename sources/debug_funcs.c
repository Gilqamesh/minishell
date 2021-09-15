/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 19:42:41 by edavid            #+#    #+#             */
/*   Updated: 2021/09/15 17:46:32 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

void	printStrArr(char **strArr)
{
	int	i;
	if (strArr == NULL)
		return ;
	i = -1;
	while (strArr[++i])
		ft_printf("%s ", strArr[i]);
	ft_printf("\n");
}

void	printSimpleCmd(t_simpleCmd *simpleCmd)
{
	ft_printf("Command: ");
	printStrArr(simpleCmd->arguments);
	ft_printf("Infile: %s ", simpleCmd->FDs.inFile.filename);
	if (simpleCmd->FDs.inFile.filename)
	{
		if (simpleCmd->FDs.inFile.mode == REDIR_IN)
			ft_printf("| mode: input");
		else if (simpleCmd->FDs.inFile.mode == REDIR_HEREDOC)
			ft_printf("| mode: heredoc");
	}
	ft_printf("\n");
	ft_printf("Outfile: %s ", simpleCmd->FDs.outFile.filename);
	if (simpleCmd->FDs.outFile.filename)
	{
		if (simpleCmd->FDs.outFile.mode == REDIR_OUT)
			ft_printf("| mode: output");
		else if (simpleCmd->FDs.outFile.mode == REDIR_APPEND)
			ft_printf("| mode: append");
	}
	ft_printf("\n");
}

void	printNodes(t_minishell *mystruct)
{
	t_simpleCmd	*tmp;

	tmp = mystruct->nodes;
	ft_printf("Simple commands:\n");
	while (tmp)
	{
		printSimpleCmd(tmp);
		tmp = tmp->next;
	}
}

void	printPipelines(t_minishell *mystruct)
{
	t_list	*cur;

	cur = mystruct->pipeLines;
	ft_printf("Pipelines:\n");
	while (cur)
	{
		ft_printf("PIPELINE START\n");
		printSimpleCmd((t_simpleCmd *)cur->content);
		ft_printf("PIPELINE END\n");
		cur = cur->next;
	}
}
