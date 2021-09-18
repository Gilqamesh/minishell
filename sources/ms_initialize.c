/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 14:35:43 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

void sighandler(int sig)
{
	if ((getMystruct(NULL))->lastPID != 0)
	{
		kill((getMystruct(NULL))->lastPID, sig);
	}
	else if (sig == SIGINT)
	{
		*rl_line_buffer = '\0';
		rl_point = 0;
		rl_end = 0;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_redisplay();
		ft_putstr_fd("hello, please enter a command line: ", STDOUT_FILENO);
	}
}

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
	mystruct->lastPID = 0;
}
