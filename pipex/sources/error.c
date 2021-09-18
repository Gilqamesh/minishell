/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:42:39 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 15:28:18 by edavid           ###   ########.fr       */
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
	destroy_mystruct(mystruct);
	ft_putstr_fd(message, STDERR_FILENO);
	return (1);
}

/*
** Frees all allocated memory associated with 'mystruct'
*/
void	destroy_mystruct(t_pipex *mystruct)
{
	if (mystruct->file[1])
		close(mystruct->file[1]);
	closePreviousPipes(mystruct, mystruct->nOfCmds);
	ft_lstmallocfree(&mystruct->alloced_lst);
}

/*
** Helper function for initialize_mystruct in initialize.c
*/
int	initOutFile(t_pipex *mystruct)
{
	if (mystruct->last->FDs.outFile.mode == REDIR_OUT)
		mystruct->file[1] = open(mystruct->last->arguments[0], O_WRONLY
			| O_CREAT | O_TRUNC, 0777);
	else if (mystruct->last->FDs.outFile.mode == REDIR_APPEND)
		mystruct->file[1] = open(mystruct->last->arguments[0], O_APPEND
		| O_CREAT, 0777);
	else
		return (terminate_pipex(mystruct, "Not supported FD mode.\n"));
	if (mystruct->file[1] == -1)
		return (terminate_pipex(mystruct, "Could not open outfile\n"));
	return (0);
}
