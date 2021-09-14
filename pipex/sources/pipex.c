/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:42:42 by edavid            #+#    #+#             */
/*   Updated: 2021/09/14 15:37:57 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ft_minishell.h"

static void	sendEOFtoInput(t_pipex *mystruct)
{
	if (pipe(mystruct->tmpFd) == -1)
		error_handler(mystruct, PIPEX_EPIPE, "pipe() failed\n");
	mydup2(mystruct, mystruct->tmpFd[0], STDIN_FILENO);
	close(mystruct->tmpFd[0]);
	close(mystruct->tmpFd[1]);
}

static void	handleChildProcess(t_pipex *mystruct, int curPipeNum, char *envp[])
{
	closePipe(mystruct, curPipeNum, 0);
	if (mystruct->openPipes[curPipeNum - 1][0])
	{
		mydup2(mystruct, mystruct->pipes[curPipeNum - 1][0], STDIN_FILENO);
		closePipe(mystruct, curPipeNum - 1, 0);
	}
	else
		sendEOFtoInput(mystruct);
	mydup2(mystruct, mystruct->pipes[curPipeNum][1], STDOUT_FILENO);
	closePipe(mystruct, curPipeNum, 1);
	if (execve(mystruct->commands[curPipeNum][0],
		mystruct->commands[curPipeNum], envp) == -1)
		error_handler(mystruct, PIPEX_ECMD, "command not found\n");
}

/*
** Makes a pipe between two commands
** And waits for the previous child process
*/
static void	createPipe_betweenTwoCmds(t_minishell *minishellStruct,
t_pipex *mystruct, int curPipeNum, char *envp[])
{
	pid_t	pid;
	int		wstatus;

	closePipe(mystruct, curPipeNum - 1, 1);
	wstatus = wait_childProcess();
	minishellStruct->fgExitStatus = wstatus;
	if (wstatus)
		closePipe(mystruct, curPipeNum - 1, 0);
	openPipe(mystruct, curPipeNum);
	pid = myfork(mystruct);
	if (pid == 0)
		handleChildProcess(mystruct, curPipeNum, envp);
	else
		minishellStruct->lastPID = pid;
	closePipe(mystruct, curPipeNum - 1, 0);
	closePipe(mystruct, curPipeNum, 1);
}

int	wait_childProcess(void)
{
	pid_t	pid;
	int		wstatus;

	pid = wait(&wstatus);
	if (pid == -1)
		return (PIPEX_ERR);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (WTERMSIG(wstatus));
	return (0);
}

/*
** Usage is the same as in the project pipex
*/
int	ft_pipex(t_minishell *minishellStruct, int argc, char *argv[], char *envp[])
{
	pid_t	pid;
	int		i;
	t_pipex	mystruct;
	int		statusCode;

	initialize_mystruct(argc, argv, envp, &mystruct);
	openPipe(&mystruct, 0);
	pid = myfork(&mystruct);
	if (pid == 0)
		handle_inputFile_firstCmd(&mystruct, argv, envp);
	else
		minishellStruct->lastPID = pid;
	i = 0;
	while (++i < mystruct.nOfCmds)
		createPipe_betweenTwoCmds(minishellStruct, &mystruct, i, envp);
	statusCode = handle_lastCmd_outputFile(&mystruct);
	minishellStruct->fgExitStatus = statusCode;
	destroy_mystruct(&mystruct);
	return (statusCode);
}
