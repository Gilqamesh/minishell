/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/15 17:42:42 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static void	init_envp(t_minishell *mystruct)
{
	char	*path;

	path = getenv("PATH");
	if (path)
		ft_objlstadd_front(&mystruct->envpLst, ft_objlst_new(ft_strdup("PATH"),
			ft_strdup(path)));
}

/*
** Initialize 'mystruct'
*/
void	init_mystruct(t_minishell *mystruct)
{
	ft_bzero(mystruct, sizeof(*mystruct));
	init_envp(mystruct);
}

/*
** Clears variables in 'mystruct' and prepares for next command line.
*/
void	clearStruct(t_minishell *mystruct)
{
	if (mystruct->promptStr)
	{
		free(mystruct->promptStr);
		mystruct->promptStr = NULL;
	}
	ft_destroy_str_arr(&mystruct->tokens);
	ft_simpleCmdclear(&mystruct->nodes, ft_simpleCmddel);
	// ft_lstclear(&mystruct->pipeLines, ft_simpleCmddel);
	mystruct->pipeLines = NULL;
	ft_lstclear(&mystruct->allocedPointers, ft_lstdel);
}
