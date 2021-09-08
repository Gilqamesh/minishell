/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 15:58:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/08 15:58:23 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Returns true if 'c' is part of 'set', otherwise false.
*/
bool	isCharPartOfSet(char c, char *set)
{
	if (set == NULL)
		return (false);
	while (*set)
		if (c == *set++)
			return (true);
	return (false);
}
