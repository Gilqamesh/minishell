/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:50:28 by edavid            #+#    #+#             */
/*   Updated: 2021/09/22 18:38:17 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ft_minishell.h"

static void	redirectInputFromFile(t_pipex *mystruct)
{
	mydup2(mystruct, mystruct->first->FDs.inFile.fd, STDIN_FILENO);
	close(mystruct->first->FDs.inFile.fd);
}

/*
** Creates connection between input file and first CMD then runs execve on CMD
*/
void	handle_inputFile_firstCmd(t_minishell *minishellStruct,
t_pipex *mystruct)
{
	closePipe(mystruct, 0, 0);
	if (mystruct->first->FDs.inFile.mode == REDIR_VOID)
	{
		if (pipe(mystruct->hereDocPipe) == -1)
			error_handler(mystruct, PIPEX_EPIPE, "pipe() failed\n");
		redirect_stdin(mystruct);
	}
	else if (mystruct->first->FDs.inFile.mode == REDIR_IN)
		redirectInputFromFile(mystruct);
	mydup2(mystruct, mystruct->pipes[0][1], STDOUT_FILENO);
	closePipe(mystruct, 0, 1);
	if (mystruct->first->FDs.inFile.mode == REDIR_HEREDOC)
		read_until_delimiter(mystruct);
	if (mystruct->first->isBuiltin == true)
		executeBuiltin(minishellStruct, mystruct->first->arguments, true,
			(t_std_FDs){(t_FD){NULL, STDIN_FILENO,
			mystruct->first->FDs.inFile.mode},
			(t_FD){NULL, STDOUT_FILENO, REDIR_NONE},
			(t_FD){NULL, STDERR_FILENO, REDIR_NONE}});
	if (execve(mystruct->first->arguments[0], mystruct->first->arguments,
			mystruct->envp) == -1)
		error_handler(mystruct, PIPEX_ECMD, "%s: command not found\n",
			mystruct->first->arguments[0]);
}

/*
** Transfers the data read from last CMD to output file through a pipe
*/
static int	transfer_data(t_pipex *mystruct)
{
	char	*line;
	int		ret;
	int		fd;

	fd = STDOUT_FILENO;
	if (mystruct->last->FDs.outFile.mode == REDIR_OUT
		|| mystruct->last->FDs.outFile.mode == REDIR_APPEND)
		fd = mystruct->last->FDs.outFile.fd;
	while (1)
	{
		ret = get_next_line(mystruct->pipes[mystruct->nOfCmds - 1][0], &line);
		if (ret == -1)
			return (terminate_pipex(mystruct, "get_next_line() failed\n"));
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			free(line);
			return (terminate_pipex(mystruct, "write() failed\n"));
		}
		free(line);
		if (ret && write(fd, "\n", 1) == -1)
			return (terminate_pipex(mystruct, "write() failed\n"));
		if (ret == 0)
			break ;
	}
	return (0);
}

int	handle_lastCmd_outputFile(t_pipex *mystruct)
{
	int	statusCode;

	closePipe(mystruct, mystruct->nOfCmds - 1, 1);
	statusCode = wait_childProcess();
	if (mystruct->last->FDs.outFile.mode != REDIR_VOID)
		transfer_data(mystruct);
	return (statusCode);
}

/*
** Closes open pipes up to but excluding 'upToPipeNum'
*/
void	closePreviousPipes(t_pipex *mystruct, int upToPipeNum)
{
	int	i;

	if (mystruct->openPipes == NULL)
		return ;
	i = -1;
	while (++i < upToPipeNum)
	{
		closePipe(mystruct, i, 0);
		closePipe(mystruct, i, 1);
	}
}
