/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 18:17:46 by edavid            #+#    #+#             */
/*   Updated: 2021/09/22 15:34:22 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Executes the pipeline with ft_pipex() in 'mystruct'->pipeLines
*/
int	executor(t_minishell *mystruct)
{
	t_list		*cur;

	if (ft_simpleCmdsize(mystruct->nodes) == 1
		&& isStrBuiltin(mystruct->nodes->arguments[0]))
	{
		executeBuiltin(mystruct, mystruct->nodes->arguments, false,
			((t_simpleCmd *)mystruct->pipeLines->content)->FDs);
		return (0);
	}
	cur = mystruct->pipeLines;
	while (cur)
	{
		ft_pipex(mystruct, (t_simpleCmd *)cur->content);
		cur = cur->next;
	}
	return (0);
}
