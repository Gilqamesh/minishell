/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/10 18:27:57 by edavid           ###   ########.fr       */
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
	if (mystruct->tokens)
		ft_destroy_str_arr(&mystruct->tokens);
	if (mystruct->nodes)
		ft_shell_node_clear(&mystruct->nodes, ft_shell_node_del);
	if (mystruct->inFile)
	{
		free(mystruct->inFile);
		mystruct->inFile = NULL;
	}
	if (mystruct->outFile)
	{
		free(mystruct->outFile);
		mystruct->outFile = NULL;
	}
	if (mystruct->errFile)
	{
		free(mystruct->errFile);
		mystruct->errFile = NULL;
	}
}
