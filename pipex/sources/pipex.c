/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:42:42 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 15:19:10 by edavid           ###   ########.fr       */
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

static void	handleChildProcess(t_minishell *minishellStruct, t_pipex *mystruct,
int curPipeNum)
{
	t_simpleCmd	*cur;

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
	cur = getSimpleCmdIndex(mystruct->first, curPipeNum);
	if (cur->isBuiltin == true)
		executeBuiltin(minishellStruct, cur->arguments);
	if (execve(cur->arguments[0], cur->arguments, mystruct->envp) == -1)
		error_handler(mystruct, PIPEX_ECMD, "command not found\n");
}

/*
** Makes a pipe between two commands
** And waits for the previous child process
*/
static int	createPipe_betweenTwoCmds(t_minishell *minishellStruct,
t_pipex *mystruct, int curPipeNum)
{
	pid_t	pid;
	int		wstatus;

	if (closePipe(mystruct, curPipeNum - 1, 1))
		return (1);
	wstatus = wait_childProcess();
	minishellStruct->fgExitStatus = wstatus;
	if (wstatus)
		if (closePipe(mystruct, curPipeNum - 1, 0))
			return (1);
	if (openPipe(mystruct, curPipeNum))
		return (1);
	pid = myfork(mystruct);
	if (pid == -1)
		return (1);
	if (pid == 0)
		handleChildProcess(minishellStruct, mystruct, curPipeNum);
	else
		minishellStruct->lastPID = pid;
	if (closePipe(mystruct, curPipeNum - 1, 0))
		return (1);
	if (closePipe(mystruct, curPipeNum, 1))
		return (1);
	return (0);
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
** Usage currently: "./pipex" "infile" "cmd1" "cmd2" "..." "outfile"
*/
void	ft_pipex(t_minishell *minishellStruct, t_simpleCmd *pipeLine)
{
	pid_t	pid;
	int		i;
	t_pipex	mystruct;
	int		statusCode;

	if (initialize_mystruct(minishellStruct, &mystruct, pipeLine))
		return ;
	if (openPipe(&mystruct, 0))
		return ;
	pid = myfork(&mystruct);
	if (pid == -1)
		return ;
	if (pid == 0)
		handle_inputFile_firstCmd(minishellStruct, &mystruct);
	else
		minishellStruct->lastPID = pid;
	i = 0;
	while (++i < mystruct.nOfCmds)
		if (createPipe_betweenTwoCmds(minishellStruct, &mystruct, i))
			return ;
	statusCode = handle_lastCmd_outputFile(&mystruct);
	if (statusCode != -1)
		minishellStruct->fgExitStatus = statusCode;
	destroy_mystruct(&mystruct);
}
