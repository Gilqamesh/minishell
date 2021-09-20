/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_syntax2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 19:21:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 19:26:07 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static int	handleLessThan(char *str)
{
	str++;
	if (*str == '\0')
		return (0);
	if (*str != '<')
	{
		ft_putstr_fd("parse error near `<'\n", STDERR_FILENO);
		return (1);
	}
	str++;
	if (*str != '\0')
	{
		ft_putstr_fd("parse error near `<<'\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	handleGreaterThan(char *str)
{
	str++;
	if (*str == '\0')
		return (0);
	if (*str != '>')
	{
		ft_putstr_fd("parse error near `>'\n", STDERR_FILENO);
		return (1);
	}
	str++;
	if (*str != '\0')
	{
		ft_putstr_fd("parse error near `>>'\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	isOperatorSyntaxErr(char *str)
{
	if (*str == '<')
		return (handleLessThan(str));
	else if (*str == '>')
		return (handleGreaterThan(str));
	else if (*str == '|' && *(str + 1) != '\0')
	{
		ft_putstr_fd("parse error near `|'\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
