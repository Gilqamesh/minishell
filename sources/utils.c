/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 15:58:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 14:31:27 by edavid           ###   ########.fr       */
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
** Allocates and copies the content of 'src' into 'dest'.
** If src->filename == NULL, then old->filename = ft_strdup("");
** The above is contention in ft_pipex() to send an to the corresponding FD.
*/
void	copy_FD(t_std_FDs *dest, t_std_FDs *src)
{
	if (src->inFile.filename == NULL)
		dest->inFile.filename = ft_strdup("");
	else
		dest->inFile.filename = ft_strdup(src->inFile.filename);
	dest->inFile.fd = src->inFile.fd;
	dest->inFile.mode = src->inFile.mode;
	if (src->outFile.filename == NULL)
		dest->outFile.filename = ft_strdup("");
	else
		dest->outFile.filename = ft_strdup(src->outFile.filename);
	dest->outFile.fd = src->outFile.fd;
	dest->outFile.mode = src->outFile.mode;
	if (src->errFile.filename == NULL)
		dest->errFile.filename = ft_strdup("");
	else
		dest->errFile.filename = ft_strdup(src->errFile.filename);
	dest->errFile.fd = src->errFile.fd;
	dest->errFile.mode = src->errFile.mode;
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
** exit() the process at the end.
*/
void	executeBuiltin(t_minishell *mystruct, char **commandArgs)
{
	(void)mystruct;
	if (!ft_strcmp(commandArgs[0], "echo"))
		builtin_echo(commandArgs);
	exit(EXIT_SUCCESS);
}
