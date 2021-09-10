/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/10 11:22:44 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static void	init_envp(t_minishell *mystruct)
{
	ft_nodbinadd_front(&mystruct->envpLst, ft_nodbinnew(ft_strjoin("PATH=",
		getenv("PATH"))));
	mystruct->envp = ft_nodbinstr_to_strarr(mystruct->envpLst);
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
