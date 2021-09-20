/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2021/09/20 20:16:52 by edavid           ###   ########.fr       */
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
		parseCmdLine(&mystruct);
	}
	return (0);
}
