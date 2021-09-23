/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 19:15:58 by edavid            #+#    #+#             */
/*   Updated: 2021/09/23 12:31:34 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static int	printExportedVars(t_minishell *mystruct, t_std_FDs FDs)
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
	tmp = mystruct->exportedVars;
	while (tmp != NULL)
	{
		ft_putstr_fd(tmp->key, FDs.outFile.fd);
		ft_putstr_fd("=", FDs.outFile.fd);
		ft_putendl_fd(tmp->value, FDs.outFile.fd);
		tmp = tmp->next;
	}
	return (0);
}

static int	notValidIdentifier(char *str, int errStream, char *key)
{
	ft_putstr_fd("export: `", errStream);
	ft_putstr_fd(str, errStream);
	ft_putendl_fd("': not a valid identifier", errStream);
	if (key)
		free(key);
	return (1);
}

static int	checkIfValidIdentifier(char *str, int errStream)
{
	if (ft_isValidBashIdentifier(str) == false)
		return (notValidIdentifier(str, errStream, NULL));
	return (1);
}

int	builtin_export(t_minishell *mystruct, char **commandArgs, t_std_FDs FDs)
{
	char		*key;
	char		*index;
	t_obj_lst	*cur;

	if (commandArgs[1] == NULL)
		return (printExportedVars(mystruct, FDs));
	index = ft_strchr(commandArgs[1], '=');
	if (index == NULL)
		return (checkIfValidIdentifier(commandArgs[1], FDs.errFile.fd));
	key = ft_substr(commandArgs[1], 0, index - commandArgs[1]);
	if (ft_isValidBashIdentifier(key) == false)
		return (notValidIdentifier(commandArgs[1], FDs.errFile.fd, key));
	cur = ft_objlst_findbykey(mystruct->envpLst, key);
	if (cur)
	{
		free(cur->value);
		cur->value = ft_substr(index + 1, 0, ft_strlen(index + 1));
	}
	else
		ft_objlstadd_front(&mystruct->envpLst, ft_objlst_new(ft_strdup(key),
				ft_substr(index + 1, 0, ft_strlen(index + 1))));
	free(key);
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
