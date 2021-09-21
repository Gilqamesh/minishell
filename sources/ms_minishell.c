/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2021/09/21 17:16:32 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	mystruct;

	(void)argc;
	(void)argv;
	getMystruct(&mystruct);
	init_mystruct(&mystruct, envp);
	while (1)
	{
		mystruct.promptStr = myreadline(CMD_PROMPT);
		if (mystruct.promptStr == NULL)
			break ;
		parseCmdLine(&mystruct);
	}
	return (0);
}
