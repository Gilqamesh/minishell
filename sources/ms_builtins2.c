/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 19:15:58 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 17:49:11 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

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
