/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/09 19:25:08 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static void	init_envp(t_minishell *mystruct)
{
	ft_nodbinadd_front(&mystruct->envpLst, ft_nodbinnew(ft_strjoin("PATH=",
		getenv("PATH"))));
	mystruct->envp = ft_nodbinstr_to_strarr(mystruct->envpLst);
}

/*
** Initialize 'mystruct'
*/
void	init_mystruct(t_minishell *mystruct)
{
	ft_bzero(mystruct, sizeof(*mystruct));
	init_envp(mystruct);
}
