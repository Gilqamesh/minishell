/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 17:34:16 by gohar             #+#    #+#             */
/*   Updated: 2021/09/18 14:33:10 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	builtin_echo(char **commandArgs)
{
	int		ret;
	char	*line;
	bool	havePrevious;

	if (commandArgs == NULL)
		return (0);
	havePrevious = false;
	while (1)
	{
		ret = get_next_line(STDIN_FILENO, &line);
		if (ret == -1)
			return (1);
		if (havePrevious == true)
		{
			if (ret == 1)
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (ret == 0 && commandArgs[1]
				&& ft_strcmp(commandArgs[1], "-n"))
			{
				ft_putendl_fd(line, STDOUT_FILENO);
				free(line);
				break ;
			}
		}
		ft_putstr_fd(line, STDOUT_FILENO);
		havePrevious = true;
		free(line);
		if (ret == 0)
			break ;
	}
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
