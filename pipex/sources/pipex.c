/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 14:42:42 by edavid            #+#    #+#             */
/*   Updated: 2021/09/16 19:44:43 by edavid           ###   ########.fr       */
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
		handleChildProcess(mystruct, curPipeNum, mystruct->envp);
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
** Usage is similar as in the project pipex.
** 'FDs' is the whole pipeline's infile and outfile.
** If infile is "", then infile is an empty file.
** If outfile is "", then the output of the last child is lost.
*/
void	ft_pipex(t_minishell *minishellStruct, char *argv[], t_std_FDs *FDs)
{
	pid_t	pid;
	int		i;
	t_pipex	mystruct;
	int		statusCode;

	ft_bzero(&mystruct, sizeof(mystruct));
	i = -1;
	while (argv[++i])
		;
	mystruct.argc = i;
	mystruct.argv = argv;
	mystruct.envp = minishellStruct->envp;
	if (initialize_mystruct(&mystruct, FDs))
		return ;
	if (openPipe(&mystruct, 0))
		return ;
	pid = myfork(&mystruct);
	if (pid == -1)
		return ;
	if (pid == 0)
		handle_inputFile_firstCmd(&mystruct, FDs);
	else
		minishellStruct->lastPID = pid;
	i = 0;
	while (++i < mystruct.nOfCmds)
		if (createPipe_betweenTwoCmds(minishellStruct, &mystruct, i))
			return ;
	statusCode = handle_lastCmd_outputFile(&mystruct, FDs);
	if (statusCode != -1)
		minishellStruct->fgExitStatus = statusCode;
	destroy_mystruct(&mystruct);
	// char buf[101];
	// PRINT_HERE();
	// int ret = read(STDIN_FILENO, buf, 100);
	// printf("ret: %d\n", ret);
	// buf[ret] = '\0';
	// printf("%s\n", buf);
	// perror(NULL);
	// i = -1;
	// while (++i < 10)
	// 	printf("%d ", buf[i]);
	// printf("\n");
	// PRINT_HERE();
}
