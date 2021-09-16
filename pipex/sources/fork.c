/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:50:28 by edavid            #+#    #+#             */
/*   Updated: 2021/09/16 19:36:25 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ft_minishell.h"

static void	redirectInputFromFile(t_pipex *mystruct)
{
	mydup2(mystruct, mystruct->file[0], STDIN_FILENO);
	if (close(mystruct->file[0]) == -1)
		error_handler(mystruct, PIPEX_EFCLOSE, "close() failed");
}

/*
** Creates connection between input file and first CMD then runs execve on CMD
*/
void	handle_inputFile_firstCmd(t_pipex *mystruct, t_std_FDs *FDs)
{
	closePipe(mystruct, 0, 0);
	if (!ft_strcmp(FDs->inFile.filename, ""))
	{
		if (pipe(mystruct->hereDocPipe) == -1)
			error_handler(mystruct, PIPEX_EPIPE, "pipe() failed\n");
		redirect_stdin(mystruct);
	}
	else if (!mystruct->isHereDoc)
	{
		mystruct->file[0] = open(mystruct->argv[1], O_RDONLY);
		if (mystruct->file[0] == -1)
			error_handler(mystruct, PIPEX_EFOPEN, "Could not open infile\n");
		redirectInputFromFile(mystruct);
	}
	mydup2(mystruct, mystruct->pipes[0][1], STDOUT_FILENO);
	closePipe(mystruct, 0, 1);
	if (mystruct->isHereDoc)
		read_until_delimiter(mystruct);
	if (execve(mystruct->commands[0][0], mystruct->commands[0], 
		mystruct->envp) == -1)
		error_handler(mystruct, PIPEX_ECMD, "command not found\n");
}

/*
** Transfers the data read from last CMD to output file through a pipe
*/
static void	transfer_data(t_pipex *mystruct)
{
	char	*line;
	int		ret;
	int		fd;

	if (ft_strcmp(mystruct->argv[mystruct->argc - 1], ""))
		fd = mystruct->file[1];
	else
		fd = STDOUT_FILENO;
	while (1)
	{
		ret = get_next_line(STDIN_FILENO, &line);
		if (ret == -1)
			error_handler(mystruct, PIPEX_ERR, "get_next_line() failed\n");
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			free(line);
			error_handler(mystruct, PIPEX_ERR, "write() failed at line\n");
		}
		free(line);
		if (ret)
			if (write(fd, "\n", 1) == -1)
				error_handler(mystruct, PIPEX_ERR, "write() failed\n");
		if (ret == 0)
			break ;
	}
	if (ft_strcmp(mystruct->argv[mystruct->argc - 1], "") && close(fd) == -1)
		error_handler(mystruct, PIPEX_EFCLOSE, "close() failed\n");
}

int	handle_lastCmd_outputFile(t_pipex *mystruct, t_std_FDs *FDs)
{
	int	statusCode;

	closePipe(mystruct, mystruct->nOfCmds - 1, 1);
	statusCode = wait_childProcess();
	mydup2(mystruct, mystruct->pipes[mystruct->nOfCmds - 1][0], STDIN_FILENO);
	if (closePipe(mystruct, mystruct->nOfCmds - 1, 0))
		return (-1);
	if (FDs->outFile.mode != REDIR_VOID)
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
