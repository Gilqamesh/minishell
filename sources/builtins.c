/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gohar <gohar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:34:16 by gohar             #+#    #+#             */
/*   Updated: 2021/09/13 14:48:34 by gohar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int builtin_echo(char *input, int outstream)
{
	char *tmp;
	int nl;

	nl = 1;
	tmp = ft_trim_from_left_index(input, " \t\n");
	if (ft_strncmp(tmp, "-n", 2) == 0)
		nl = 0;
	ft_putstr_fd(input, outstream);
	if (nl)
		ft_putchar_fd('\n', outstream);
	return (1);
}

int builtin_export(t_minishell *mystruct, char *key, char *val)
{
	t_obj_lst *tmp;

	tmp = mygetenv(mystruct, key);
	if (tmp == NULL)
		ft_objlstadd_front(&(mystruct->envpLst), ft_objlst_new(key, val));
	else
	{
		free(tmp->value);
		tmp->value = val;
	}
	return (1);
}

int builtin_unset(t_minishell *mystruct, char *key)
{
	t_obj_lst *tmp;

	tmp = mygetenv(mystruct, key);
	if (tmp == NULL)
		return 1;
	else
		ft_objlst_rem(&(mystruct->envpLst), tmp);
	return (1);
}

int builtin_cd(t_minishell *mystruct, char *input)
{
	int flag;

	if (ft_strcmp(input, "..") == 0)
		input = getenvvar("OLDPWD");
	flag = chdir(input);
	if (flag < 0)
		return (0);
	builtin_export(mystruct, "OLDPWD", getenvvar("PWD"));
	builtin_export(mystruct, "PWD", input);
	return (1);
}

int builtin_pwd(t_minishell *mystruct, int outstream)
{
	t_obj_lst *tmp;

	tmp = mygetenv(mystruct, "PWD");
	ft_putstr_fd(tmp->value, outstream);

	return (1);
}

int builtin_env(t_minishell *mystruct, int outstream)
{
	t_obj_lst *tmp;

	tmp = mystruct->envpLst;
	while(tmp != NULL)
	{
		ft_putstr_fd(tmp->key, outstream);
		ft_putstr_fd("=",outstream);
		ft_putstr_fd(tmp->value, outstream);
		tmp = tmp->next;
	}
	return (1);
}
