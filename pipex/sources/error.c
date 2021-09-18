/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:42:39 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 19:06:46 by edavid           ###   ########.fr       */
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
	if (mystruct->file[1] && mystruct->file[1] != -1
		&& mystruct->file[1] != STDOUT_FILENO)
		close(mystruct->file[1]);
	if (mystruct->file[0] && mystruct->file[0] != -1
		&& mystruct->file[0] != STDIN_FILENO)
		close(mystruct->file[0]);
	closePreviousPipes(mystruct, mystruct->nOfCmds);
	ft_destroy_str_arr(&mystruct->envp);
	ft_lstmallocfree(&mystruct->alloced_lst);
}
