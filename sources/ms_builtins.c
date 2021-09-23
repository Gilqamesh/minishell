/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:34:16 by gohar             #+#    #+#             */
/*   Updated: 2021/09/23 13:43:33 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

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

static int	handleHome(t_minishell *mystruct, char **commandArgs, char **str)
{
	t_obj_lst	*tmp;

	*str = commandArgs[1];
	if (*str == NULL)
	{
		tmp = ft_objlst_findbykey(mystruct->envpLst, "HOME");
		if (tmp == NULL)
			return (1);
		*str = tmp->value;
	}
	return (0);
}

static int	printNoSuchFile(char *tmpStr)
{
	
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(tmpStr, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	return (1);
}

int	builtin_cd(t_minishell *mystruct, char **commandArgs)
{
	char		*path;
	char		*tmpStr;
	t_obj_lst	*curPwd;
	t_obj_lst	*oldPwd;

	if (handleHome(mystruct, commandArgs, &tmpStr)
		|| ft_strcmp(tmpStr, ".") == 0)
		return (1);
	curPwd = ft_objlst_findbykey(mystruct->envpLst, "PWD");
	oldPwd = ft_objlst_findbykey(mystruct->envpLst, "OLDPWD");
	if (chdir(tmpStr) == -1)
		return (printNoSuchFile(tmpStr));
	path = setPath(tmpStr, curPwd);
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
