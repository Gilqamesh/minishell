/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2021/09/07 17:21:50 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	main(void)
{
	char		 *str;
	t_minishell	mystruct;

	while(1)
	{
		str = myreadline("hello, please enter a command line: ");
			if (str == NULL)
				continue ;
		parseCmdLine(&mystruct, str);
		free(str);
	}
	return (0);
}
