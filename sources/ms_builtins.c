/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:34:16 by gohar             #+#    #+#             */
/*   Updated: 2021/09/20 20:41:09 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	builtin_echo(char **commandArgs, t_3_int in_out_streams)
{
	if (in_out_streams.a == REDIR_IN && in_out_streams.b == -1)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	if (commandArgs == NULL || commandArgs[0] == NULL || commandArgs[1] == NULL)
		return (1);
	if (!ft_strcmp(commandArgs[1], "-n"))
		ft_putstr_fd(commandArgs[2], in_out_streams.c);
	else
		ft_putendl_fd(commandArgs[1], in_out_streams.c);
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

int	builtin_pwd(t_minishell *mystruct, t_3_int in_out_streams)
{
	t_obj_lst	*tmp;

	if (in_out_streams.a == REDIR_IN && in_out_streams.b == -1)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	tmp = ft_objlst_findbykey(mystruct->envpLst, "PWD");
	if (tmp == NULL)
		return (1);
	ft_putendl_fd(tmp->value, in_out_streams.c);
	return (0);
}
