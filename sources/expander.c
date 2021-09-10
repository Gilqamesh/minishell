/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 12:30:04 by edavid            #+#    #+#             */
/*   Updated: 2021/09/10 12:59:15 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Performs the various shell expansions, breaking the expanded tokens into
** lists of filenames and commands and arguments.
** Currently supported: Environment variables ($ followed by some characters)
*/
int	expander(t_minishell *mystruct)
{
	int		i;
	char	*tmp;
	char	*

	i = -1;
	while (mystruct->tokens[++i])
	{
		tmp = expandIndex(mystruct->tokens[i]);
		while (tmp)
		{
			
			expandStr(&mystruct->tokens[i], tmp);
			tmp = expandIndex(mystruct->tokens[i]);
		}
	}
}
