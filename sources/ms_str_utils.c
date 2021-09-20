/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_str_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:56:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 20:12:40 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Removes the last directory of the current *'path'
** example: "/home/user" -> "/home"
** Also returns *'path'
*/
char	*removeLastDirOfPath(char *path)
{
	char	*index;
	char	*newPath;

	if (path == NULL)
		return (NULL);
	index = ft_strrchr(path, '/');
	if (index == NULL || index - path < 1)
		return (ft_strdup("/"));
	newPath = ft_calloc(index - path + 1, sizeof(*newPath));
	ft_strlcpy(newPath, path, index - path + 1);
	return (newPath);
}

/*
** Returns true if 'str' is a builtin command.
*/
bool	isStrBuiltin(char *str)
{
	static char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", NULL};

	return (ft_isStrContainedInStrArr(str, builtins));
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
