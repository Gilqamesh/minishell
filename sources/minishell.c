/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsiddiqu <gsiddiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2021/09/16 18:46:23 by gsiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	main(void)
{
	t_minishell	mystruct;

	getMystruct(&mystruct);
	init_mystruct(&mystruct);
	while(1)
	{
		mystruct.promptStr = myreadline("hello, please enter a command line: ");
			if (mystruct.promptStr == NULL)
				continue ;
		if (parseCmdLine(&mystruct))
			continue ;
	}
	return (0);
}
