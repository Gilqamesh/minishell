/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:42:39 by edavid            #+#    #+#             */
/*   Updated: 2021/09/19 17:41:19 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ft_minishell.h"

void	error_handler(t_pipex *mystruct, int errcode, char *message)
{
	destroy_mystruct(mystruct);
	ft_putstr_fd(message, STDERR_FILENO);
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
