/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myfuncs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/04 19:23:43 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 13:47:12 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ft_minishell.h"

/*
** fork() with error handling
*/
pid_t	myfork(t_pipex *mystruct)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		destroy_mystruct(mystruct);
		ft_putstr_fd("fork() failed\n", STDERR_FILENO);
		return (-1);
	}
	return (pid);
}

/*
** dup2() with error handling
*/
void	mydup2(t_pipex *mystruct, int fromFd, int toFd)
{
	if (dup2(fromFd, toFd) == -1)
		error_handler(mystruct, PIPEX_EDUP2, "dup2() failed\n");
}

/*
** Opens 'pipeNumber'
** In case there is an error with pipe() it calls error_handler
*/
int	openPipe(t_pipex *mystruct, int pipeNumber)
{
	if (pipe(mystruct->pipes[pipeNumber]) == -1)
		return (terminate_pipex(mystruct, "pipe() failed\n"));
	mystruct->openPipes[pipeNumber][0] = true;
	mystruct->openPipes[pipeNumber][1] = true;
	return (0);
}

/*
** Closes 'pipeNumber' read/write end for mystruct->pipes
** In case there is an error with close() it calls error_handler
*/
int	closePipe(t_pipex *mystruct, int pipeNumber, int read_or_write_end)
{
	if (mystruct->openPipes[pipeNumber][read_or_write_end])
	{
		mystruct->openPipes[pipeNumber][read_or_write_end] = false;
		if (close(mystruct->pipes[pipeNumber][read_or_write_end]) == -1)
			return (terminate_pipex(mystruct, "close() failed\n"));
	}
	return (0);
}

/*
** Returns the 'index'th pointer to t_simpleCmd in 'lst'.
*/
t_simpleCmd	*getSimpleCmdIndex(t_simpleCmd *lst, int index)
{
	if (index < 0)
		return (lst);
	while (index--)
		lst = lst->next;
	return (lst);
}
