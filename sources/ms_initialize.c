/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsiddiqu <gsiddiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/16 17:03:47 by gsiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

void sighandler(int sig)
{
	if (data->lastPID != 0)
	{
		kill(data->lastPID, sig);
	}
	else if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		ft_putstr_fd("\n", 1);
		rl_redisplay();
		ft_putstr_fd("hello, please enter a command line: ", 1);
	}
}

static void	init_envp(t_minishell *mystruct)
{
	char	*path;

	path = getenv("PATH");
	if (path)
	{
		ft_objlstadd_front(&mystruct->envpLst, ft_objlst_new(ft_strdup("PATH"),
			ft_strdup(path)));
		ft_appendStrArr(&mystruct->envp, path);
	}
}

/*
** Initialize 'mystruct'
*/
void	init_mystruct(t_minishell *mystruct)
{
	signal(SIGINT, &sighandler);
	signal(SIGQUIT, &sighandler);
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
