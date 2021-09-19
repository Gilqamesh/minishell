/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 15:58:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/19 21:41:59 by edavid           ###   ########.fr       */
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

/*
** Allocates and returns the copy of 'src'.
*/
t_std_FDs	*copy_FD(t_std_FDs *src)
{
	t_std_FDs	*new;

	new = ft_calloc(1, sizeof(*new));
	new->inFile.filename = ft_strdup(src->inFile.filename);
	new->inFile.fd = src->inFile.fd;
	new->inFile.mode = src->inFile.mode;
	new->outFile.filename = ft_strdup(src->outFile.filename);
	new->outFile.fd = src->outFile.fd;
	new->outFile.mode = src->outFile.mode;
	new->errFile.filename = ft_strdup(src->errFile.filename);
	new->errFile.fd = src->errFile.fd;
	new->errFile.mode = src->errFile.mode;
	return (new);
}

/*
** Allocates and return a 1-len string that contains the char 'c'.
** If c == '\0', NULL is returned.
*/
static char	*ft_charToStr(char c)
{
	char	*str;

	if (c == '\0')
		return (NULL);
	str = malloc(2);
	str[1] = '\0';
	str[0] = c;
	return (str);
}

/*
** Allocates a string that is the concatenation of 'strArr' with delimiter char
** in between each string from 'strArr'.
*/
char	*ft_strArrtoStr(char **strArr, char delimiter)
{
	int				i;
	t_node_binary	*lst;
	char			*resStr;

	if (strArr == NULL || *strArr == NULL)
		return (NULL);
	lst = NULL;
	i = -1;
	while (strArr[++i])
	{
		if (strArr[i + 1])
			ft_nodbinadd_front(&lst, ft_nodbinnew(ft_strjoin_free(
				ft_strdup(strArr[i]), ft_charToStr(delimiter))));
		else
			ft_nodbinadd_front(&lst, ft_nodbinnew(ft_strjoin_free(
				ft_strdup(strArr[i]), ft_charToStr('\0'))));
	}
	resStr = ft_nodbinstrjoin_from_back(lst);
	ft_nodbinclear(&lst, ft_nodbindel, -1);
	return (resStr);
}

/*
** Puts 'str' at the end of the NULL terminated *'strArrPtr'.
*/
void	ft_appendStrArr(char ***strArrPtr, char *str)
{
	int		len;
	int		i;
	char	**new;

	if (strArrPtr == NULL)
		return ;
	if (*strArrPtr == NULL)
	{
		*strArrPtr = ft_calloc(2, sizeof(**strArrPtr));
		(*strArrPtr)[0] = ft_strdup(str);
		return ;
	}
	len = -1;
	while ((*strArrPtr)[++len])
		;
	new = ft_calloc(len + 2, sizeof(*new));
	i = -1;
	while (++i < len)
		new[i] = (*strArrPtr)[i];
	new[i] = ft_strdup(str);
	free(*strArrPtr);
	*strArrPtr = new;
}

/*
** Yes.
*/
t_minishell	*getMystruct(t_minishell *mystruct)
{
	static t_minishell	*ptr;
	static bool			called = true;

	if (called == true)
	{
		ptr = mystruct;
		called = false;
	}
	return (ptr);
}

/*
** Initializes 'FD' with default values for a simple command.
*/
void	initFD(t_std_FDs *FD)
{
	FD->errFile.fd = STDERR_FILENO;
	FD->errFile.filename = NULL;
	FD->errFile.mode = REDIR_NONE;
	FD->inFile.fd = STDIN_FILENO;
	FD->inFile.filename = NULL;
	FD->inFile.mode = REDIR_NONE;
	FD->outFile.fd = STDOUT_FILENO;
	FD->outFile.filename = NULL;
	FD->outFile.mode = REDIR_NONE;
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
** Executes the builtin function 'commandArgs'[0] with arguments.
** STD FDs are automatically redirected from pipex at the call of this function.
** exit() the process at the end to exit from the child process.
*/
void	executeBuiltin(t_minishell *mystruct, char **commandArgs,
bool shouldExit)
{
	(void)mystruct;
	if (!ft_strcmp(commandArgs[0], "echo"))
		builtin_echo(commandArgs);
	if (!ft_strcmp(commandArgs[0], "exit"))
		exit(EXIT_SUCCESS);
	if (!ft_strcmp(commandArgs[0], "cd"))
		builtin_cd(mystruct, commandArgs);
	// if (!ft_strcmp(commandArgs[0], "export"))
	// 	builtin_export();
	if (shouldExit == true)
		exit(EXIT_SUCCESS);
}

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
	if (index == NULL)
		return (ft_strdup("/"));
	newPath = ft_calloc(index - path + 1, sizeof(*newPath));
	ft_strlcpy(newPath, path, index - path + 1);
	return (newPath);
}
