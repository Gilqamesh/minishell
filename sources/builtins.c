/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:34:16 by gohar             #+#    #+#             */
/*   Updated: 2021/09/18 16:11:50 by edavid           ###   ########.fr       */
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

// int	builtin_export(t_minishell *mystruct, char *key, char *val)
// {
// 	t_obj_lst	*tmp;

// 	tmp = mygetenv(mystruct, key);
// 	if (tmp == NULL)
// 		ft_objlstadd_front(&(mystruct->envpLst), ft_objlst_new(key, val));
// 	else
// 	{
// 		free(tmp->value);
// 		tmp->value = val;
// 	}
// 	return (1);
// }

// int	builtin_unset(t_minishell *mystruct, char *key)
// {
// 	t_obj_lst	*tmp;

// 	tmp = mygetenv(mystruct, key);
// 	if (tmp == NULL)
// 		return (1);
// 	else
// 		ft_objlst_rem(&(mystruct->envpLst), tmp);
// 	return (1);
// }

// int	builtin_cd(t_minishell *mystruct, char *input)
// {
// 	int	flag;

// 	if (ft_strcmp(input, "..") == 0)
// 		input = getenvvar("OLDPWD");
// 	flag = chdir(input);
// 	if (flag < 0)
// 		return (0);
// 	builtin_export(mystruct, "OLDPWD", getenvvar("PWD"));
// 	builtin_export(mystruct, "PWD", input);
// 	return (1);
// }

// int	builtin_pwd(t_minishell *mystruct, int outstream)
// {
// 	t_obj_lst	*tmp;

// 	tmp = mygetenv(mystruct, "PWD");
// 	ft_putstr_fd(tmp->value, outstream);
// 	return (1);
// }

// int	builtin_env(t_minishell *mystruct, int outstream)
// {
// 	t_obj_lst	*tmp;

// 	tmp = mystruct->envpLst;
// 	while (tmp != NULL)
// 	{
// 		ft_putstr_fd(tmp->key, outstream);
// 		ft_putstr_fd("=", outstream);
// 		ft_putstr_fd(tmp->value, outstream);
// 		tmp = tmp->next;
// 	}
// 	return (1);
// }
