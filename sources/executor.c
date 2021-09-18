/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 18:17:46 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 16:09:32 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Executes the pipeline with ft_pipex() in 'mystruct'->pipeLines
*/
int	executor(t_minishell *mystruct)
{
	t_list		*cur;

	cur = mystruct->pipeLines;
	while (cur)
	{
		ft_pipex(mystruct, (t_simpleCmd *)cur->content);
		cur = cur->next;
	}
	return (0);
}
