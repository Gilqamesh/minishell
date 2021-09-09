/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isValidCName.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 16:02:48 by edavid            #+#    #+#             */
/*   Updated: 2021/09/09 18:01:58 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static bool	isStrCKeyword(char *str)
{
	static char	*keywords[] = {
		"auto", "break", "char", "const", "continue", "do", "double", "else",
		"extern", "float", "float", "for", "if", NULL
	};

	if (ft_isStrContainedInStrArr(str, keywords))
		return (false);
	return (true);
}

/*
** Returns true if 'str' is a valid C identifier, false otherwise.
*/
bool	ft_isValidCName(char *str)
{
	if (str == NULL)
		return (false);
	if (isStrCKeyword(str))
		return (false);
	if (ft_isalpha(*str) || *str == '_')
		str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (true);
}
