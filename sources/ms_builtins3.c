/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 14:15:13 by edavid            #+#    #+#             */
/*   Updated: 2021/09/22 18:54:51 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	builtin_env(t_minishell *mystruct, t_std_FDs FDs)
{
	t_obj_lst	*tmp;

	if (FDs.inFile.fd == -1)
	{
		ft_putstr_fd(FDs.inFile.filename, FDs.errFile.fd);
		ft_putstr_fd(": No such file or directory\n", FDs.errFile.fd);
		return (1);
	}
	if (FDs.inFile.mode == REDIR_HEREDOC)
		readTillDelim(FDs.inFile.filename);
	tmp = mystruct->envpLst;
	while (tmp != NULL)
	{
		ft_putstr_fd(tmp->key, FDs.outFile.fd);
		ft_putstr_fd("=", FDs.outFile.fd);
		ft_putendl_fd(tmp->value, FDs.outFile.fd);
		tmp = tmp->next;
	}
	return (0);
}

int	builtin_pwd(t_minishell *mystruct, t_std_FDs FDs)
{
	t_obj_lst	*tmp;

	if (FDs.inFile.fd == -1)
	{
		ft_putstr_fd(FDs.inFile.filename, FDs.errFile.fd);
		ft_putstr_fd(": No such file or directory\n", FDs.errFile.fd);
		return (1);
	}
	if (FDs.inFile.mode == REDIR_HEREDOC)
		readTillDelim(FDs.inFile.filename);
	tmp = ft_objlst_findbykey(mystruct->envpLst, "PWD");
	if (tmp == NULL)
		return (1);
	ft_putendl_fd(tmp->value, FDs.outFile.fd);
	return (0);
}

int	builtin_exit(t_minishell *mystruct, char **commandArgs)
{
	if (commandArgs == NULL || commandArgs[1] == NULL)
		exit(mystruct->fgExitStatus);
	if (commandArgs[2])
	{
		mystruct->fgExitStatus = 1;
		ft_putendl_fd("exit", STDERR_FILENO);
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		return (0);
	}
	if (ft_isStrNumber(commandArgs[1]) == true)
		exit((unsigned char)ft_atoi(commandArgs[1]));
	else
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(commandArgs[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(255);
	}
}

static int	handleErrEcho(char **commandArgs, t_std_FDs FDs)
{
	if (FDs.inFile.mode == REDIR_HEREDOC)
		readTillDelim(FDs.inFile.filename);
	if (commandArgs == NULL || commandArgs[0] == NULL || (commandArgs[1]
			&& ft_strcmp(commandArgs[1], "-n") == 0 && commandArgs[2] == NULL))
		return (1);
	if (FDs.inFile.fd == -1)
	{
		ft_putstr_fd(FDs.inFile.filename, FDs.errFile.fd);
		ft_putstr_fd(": No such file or directory\n", FDs.errFile.fd);
		return (1);
	}
	return (0);
}

int	builtin_echo(char **commandArgs, t_std_FDs FDs)
{
	int	i;

	if (handleErrEcho(commandArgs, FDs))
		return (1);
	if (commandArgs[1] == NULL)
	{
		ft_putchar_fd('\n', FDs.outFile.fd);
		return (0);
	}
	if (!ft_strcmp(commandArgs[1], "-n"))
		i = 1;
	else
		i = 0;
	while (commandArgs[++i + 1])
	{
		ft_putstr_fd(commandArgs[i], FDs.outFile.fd);
		ft_putchar_fd(' ', FDs.outFile.fd);
	}
	if (!ft_strcmp(commandArgs[1], "-n"))
		ft_putstr_fd(commandArgs[i], FDs.outFile.fd);
	else
		ft_putendl_fd(commandArgs[i], FDs.outFile.fd);
	return (0);
}
