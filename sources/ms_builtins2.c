/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 19:15:58 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 20:40:37 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

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

int	builtin_env(t_minishell *mystruct, t_3_int in_out_streams)
{
	t_obj_lst	*tmp;

	if (in_out_streams.a == REDIR_IN && in_out_streams.b == -1)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	tmp = mystruct->envpLst;
	while (tmp != NULL)
	{
		ft_putstr_fd(tmp->key, in_out_streams.c);
		ft_putstr_fd("=", in_out_streams.c);
		ft_putendl_fd(tmp->value, in_out_streams.c);
		tmp = tmp->next;
	}
	return (0);
}
