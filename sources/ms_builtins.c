/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:34:16 by gohar             #+#    #+#             */
/*   Updated: 2021/09/21 13:31:50 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

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

static char	*setPath(char *arg, t_obj_lst *curPwd)
{
	char	*path;

	if (ft_strcmp(arg, "..") == 0)
	{
		if (curPwd == NULL)
			path = ft_strdup("/");
		else
			path = removeLastDirOfPath(curPwd->value);
	}
	else
	{
		if (curPwd == NULL || *arg == '/')
			path = ft_strdup(arg);
		else
			path = ft_strjoin_free(ft_strjoin_free(ft_strdup(curPwd->value),
						ft_strdup("/")), ft_strdup(arg));
	}
	return (path);
}

static void	setPwd(t_obj_lst *curPwd, t_obj_lst *oldPwd, t_minishell *mystruct,
char *path)
{
	if (curPwd == NULL)
		ft_objlstadd_front(&mystruct->envpLst,
			ft_objlst_new(ft_strdup("PWD"), ft_strdup(path)));
	if (oldPwd == NULL)
		ft_objlstadd_front(&mystruct->envpLst,
			ft_objlst_new(ft_strdup("OLDPWD"), ft_strdup(path)));
}

int	builtin_cd(t_minishell *mystruct, char **commandArgs)
{
	char		*path;
	t_obj_lst	*curPwd;
	t_obj_lst	*oldPwd;

	if (commandArgs[1] == NULL || ft_strcmp(commandArgs[1], ".") == 0)
		return (1);
	curPwd = ft_objlst_findbykey(mystruct->envpLst, "PWD");
	oldPwd = ft_objlst_findbykey(mystruct->envpLst, "OLDPWD");
	if (chdir(commandArgs[1]) == -1)
		return (1);
	path = setPath(commandArgs[1], curPwd);
	if (curPwd == NULL || oldPwd == NULL)
		setPwd(curPwd, oldPwd, mystruct, path);
	else
	{
		if (oldPwd->value)
			free(oldPwd->value);
		oldPwd->value = ft_strdup(curPwd->value);
		free(curPwd->value);
		curPwd->value = path;
	}
	chdir(path);
	return (0);
}
