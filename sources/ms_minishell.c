/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2021/09/23 16:02:42 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static void	ms_destroy_mystruct(t_minishell *mystruct)
{
	ft_objlst_clear(&mystruct->envpLst, ft_objlst_del);
	ft_objlst_clear(&mystruct->exportedVars, ft_objlst_del);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	mystruct;

	(void)argc;
	(void)argv;
	getMystruct(&mystruct);
	init_mystruct(&mystruct, envp);
	while (1)
	{
		ft_ctlsigchars(&mystruct, 0);
		mystruct.promptStr = myreadline(CMD_PROMPT);
		if (mystruct.promptStr == NULL)
			break ;
		parseCmdLine(&mystruct);
	}
	ft_ctlsigchars(&mystruct, 1);
	ms_destroy_mystruct(&mystruct);
	return (0);
}
