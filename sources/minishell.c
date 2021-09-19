/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2021/09/19 16:39:49 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	main(void)
{
	t_minishell	mystruct;

	getMystruct(&mystruct);
	init_mystruct(&mystruct);
	while (1)
	{
		mystruct.promptStr = myreadline(CMD_PROMPT);
		if (mystruct.promptStr == NULL)
			break ;
		if (parseCmdLine(&mystruct))
			continue ;
	}
	return (0);
}
