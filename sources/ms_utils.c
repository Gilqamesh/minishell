/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 15:58:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 20:40:57 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Allocates and returns the copy of 'src'.
*/
t_std_FDs	copy_FD(t_std_FDs *src)
{
	t_std_FDs	new;

	new.inFile.filename = ft_strdup(src->inFile.filename);
	new.inFile.fd = src->inFile.fd;
	new.inFile.mode = src->inFile.mode;
	new.outFile.filename = ft_strdup(src->outFile.filename);
	new.outFile.fd = src->outFile.fd;
	new.outFile.mode = src->outFile.mode;
	new.errFile.filename = ft_strdup(src->errFile.filename);
	new.errFile.fd = src->errFile.fd;
	new.errFile.mode = src->errFile.mode;
	return (new);
}

/*
** Initializes 'FD' with default values for a simple command.
*/
void	initFD(t_std_FDs *FD)
{
	FD->errFile.fd = STDERR_FILENO;
	FD->errFile.filename = NULL;
	FD->errFile.mode = REDIR_NONE;
	FD->inFile.fd = STDIN_FILENO;
	FD->inFile.filename = NULL;
	FD->inFile.mode = REDIR_NONE;
	FD->outFile.fd = STDOUT_FILENO;
	FD->outFile.filename = NULL;
	FD->outFile.mode = REDIR_NONE;
}

/*
** First time call it like: getMystruct(mystruct).
** All consecutive calls of getMystruct() will return 'mystruct'.
*/
t_minishell	*getMystruct(t_minishell *mystruct)
{
	static t_minishell	*ptr;
	static bool			called = true;

	if (called == true)
	{
		ptr = mystruct;
		called = false;
	}
	return (ptr);
}

/*
** Executes the builtin function 'commandArgs'[0] with arguments.
** STD FDs are automatically redirected from pipex at the call of this function.
** exit() the process at the end to exit from the child process.
*/
void	executeBuiltin(t_minishell *mystruct, char **commandArgs,
bool shouldExit, t_3_int in_out_streams)
{
	mystruct->fgExitStatus = 0;
	if (ft_strcmp(commandArgs[0], "echo") == 0)
		builtin_echo(commandArgs, in_out_streams);
	else if (ft_strcmp(commandArgs[0], "export") == 0)
		builtin_export(mystruct, commandArgs);
	else if (ft_strcmp(commandArgs[0], "unset") == 0)
		builtin_unset(mystruct, commandArgs);
	else if (ft_strcmp(commandArgs[0], "cd") == 0)
		builtin_cd(mystruct, commandArgs);
	else if (ft_strcmp(commandArgs[0], "pwd") == 0)
		builtin_pwd(mystruct, in_out_streams);
	else if (ft_strcmp(commandArgs[0], "env") == 0)
		builtin_env(mystruct, in_out_streams);
	else if (ft_strcmp(commandArgs[0], "exit") == 0)
		exit(EXIT_SUCCESS);
	if (shouldExit == true)
		exit(EXIT_SUCCESS);
}
