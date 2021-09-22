/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:42:39 by edavid            #+#    #+#             */
/*   Updated: 2021/09/22 18:50:03 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ft_minishell.h"

void	error_handler(t_pipex *mystruct, int errcode, char *message, ...)
{
	va_list	ap;

	va_start(ap, message);
	ft_dprintf(STDERR_FILENO, message, ap);
	va_end(ap);
	destroy_mystruct(mystruct);
	exit(errcode);
}

/*
** Destroy mystruct, writes 'message' to STDERR then returns 1.
*/
int	terminate_pipex(t_pipex *mystruct, char *message)
{
	closeFDs(mystruct);
	destroy_mystruct(mystruct);
	ft_putstr_fd(message, STDERR_FILENO);
	return (1);
}

/*
** Frees all allocated memory associated with 'mystruct'
*/
void	destroy_mystruct(t_pipex *mystruct)
{
	closePreviousPipes(mystruct, mystruct->nOfCmds);
	ft_destroy_str_arr(&mystruct->envp);
	ft_lstmallocfree(&mystruct->alloced_lst);
}

/*
** Close file descriptors of in and outfile.
*/
void	closeFDs(t_pipex *mystruct)
{
	if (mystruct->last->FDs.outFile.mode == REDIR_OUT
		|| mystruct->last->FDs.outFile.mode == REDIR_APPEND)
		close(mystruct->last->FDs.outFile.fd);
}

int	pipex_init_pipes(t_pipex *mystruct)
{
	mystruct->pipes = ft_lstmallocwrapper(&mystruct->alloced_lst,
			mystruct->nOfCmds * sizeof(*mystruct->pipes), false);
	if (mystruct->pipes == NULL)
		return (1);
	mystruct->openPipes = ft_lstmallocwrapper(&mystruct->alloced_lst,
			mystruct->nOfCmds * sizeof(*mystruct->openPipes), true);
	if (mystruct->openPipes == NULL)
		return (1);
	return (0);
}
