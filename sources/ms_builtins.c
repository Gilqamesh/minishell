/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:34:16 by gohar             #+#    #+#             */
/*   Updated: 2021/09/20 18:20:22 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	builtin_echo(char **commandArgs)
{
	if (commandArgs == NULL || commandArgs[0] == NULL || commandArgs[1] == NULL)
		return (1);
	if (!ft_strcmp(commandArgs[1], "-n"))
		ft_putstr_fd(commandArgs[2], STDOUT_FILENO);
	else
		ft_putendl_fd(commandArgs[1], STDOUT_FILENO);
	return (0);
}

int	builtin_export(t_minishell *mystruct, char **commandArgs)
{
	char		*key;
	char		*index;
	t_obj_lst	*cur;

	if (commandArgs[1] == NULL)
		return (1);
	index = ft_strchr(commandArgs[1], '=');
	if (index == NULL)
		return (1);
	key = ft_substr(commandArgs[1], 0, index - commandArgs[1]);
	cur = ft_objlst_findbykey(mystruct->envpLst, key);
	if (cur)
	{
		free(cur->value);
		cur->value = ft_substr(index + 1, 0, ft_strlen(index + 1));
	}
	else
		ft_objlstadd_front(&mystruct->envpLst, ft_objlst_new(key,
			ft_substr(index + 1, 0, ft_strlen(index + 1))));
	return (0);
}

int	builtin_unset(t_minishell *mystruct, char **commandArgs)
{
	t_obj_lst	*tmp;

	if (commandArgs[1] == NULL)
		return (1);
	tmp = ft_objlst_findbykey(mystruct->envpLst, commandArgs[1]);
	if (tmp == NULL)
		return (1);
	else
		ft_objlst_rem(&mystruct->envpLst, tmp);
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
	{
		if (curPwd == NULL)
			ft_objlstadd_front(&mystruct->envpLst,
				ft_objlst_new(ft_strdup("PWD"), ft_strdup(path)));
		if (oldPwd == NULL)
			ft_objlstadd_front(&mystruct->envpLst,
				ft_objlst_new(ft_strdup("OLDPWD"), ft_strdup(path)));
	}
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

int	builtin_pwd(t_minishell *mystruct)
{
	t_obj_lst	*tmp;

	tmp = ft_objlst_findbykey(mystruct->envpLst, "PWD");
	if (tmp == NULL)
		return (1);
	ft_putendl_fd(tmp->value, STDOUT_FILENO);
	return (0);
}

int	builtin_env(t_minishell *mystruct)
{
	t_obj_lst	*tmp;

	tmp = mystruct->envpLst;
	while (tmp != NULL)
	{
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		ft_putstr_fd("=", STDOUT_FILENO);
		ft_putendl_fd(tmp->value, STDOUT_FILENO);
		tmp = tmp->next;
	}
	return (0);
}
