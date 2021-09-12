/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 15:58:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/12 16:32:20 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Returns true if 'c' is part of 'set', otherwise false.
*/
char	isCharPartOfSet(char c, char *set)
{
	if (set == NULL)
		return ('\0');
	while (*set)
	{
		if (c == *set)
			return (*set);
		set++;
	}
	return ('\0');
}
