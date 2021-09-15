/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 15:58:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/15 16:27:58 by edavid           ###   ########.fr       */
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

/*
** Allocates and returns the duplicate of the NULL terminated 'strArr'.
*/
char	**ft_strArrDup(char **strArr)
{
	int		i;
	char	**newStrArr;

	if (strArr == NULL)
		return (NULL);
	i = -1;
	while (strArr[++i])
		;
	newStrArr = ft_calloc(i + 1, sizeof(*newStrArr));
	if (newStrArr == NULL)
		return (NULL);
	i = -1;
	while (strArr[++i])
		newStrArr[i] = ft_strdup(strArr[i]);
	return (newStrArr);
}
