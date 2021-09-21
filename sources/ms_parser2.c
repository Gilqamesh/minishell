/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 19:33:07 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 19:24:09 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static void	replaceStr(char **old, char *new)
{
	if (old == NULL)
		return ;
	if (*old)
		free(*old);
	*old = ft_strdup(new);
}

static void	handleOutRedir(t_std_FDs *old, char *redirOp, char *new)
{
	if (!ft_strcmp(redirOp, ">"))
	{
		if (old->outFile.fd != STDOUT_FILENO && old->outFile.fd)
			close(old->outFile.fd);
		replaceStr(&old->outFile.filename, new);
		old->outFile.mode = REDIR_OUT;
		old->outFile.fd = open(old->outFile.filename, O_WRONLY | O_CREAT
				| O_TRUNC, 0777);
	}
	else if (!ft_strcmp(redirOp, ">>"))
	{
		if (old->outFile.fd != STDOUT_FILENO && old->outFile.fd)
			close(old->outFile.fd);
		replaceStr(&old->outFile.filename, new);
		old->outFile.mode = REDIR_APPEND;
		old->outFile.fd = open(old->outFile.filename, O_WRONLY | O_CREAT
				| O_APPEND, 0777);
	}
}

static int	failedSimpleCmd(t_std_FDs *old, int *i)
{
	ft_putstr_fd(old->inFile.filename, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	(*i)++;
	return (1);
}

int	replaceFD(t_std_FDs *old, char *redirOp, char *new, int *i)
{
	if (!ft_strcmp(redirOp, "<"))
	{
		if (old->inFile.fd != STDIN_FILENO && old->inFile.fd)
			close(old->inFile.fd);
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_IN;
		old->inFile.fd = open(old->inFile.filename, O_RDONLY);
		if (old->inFile.fd == -1)
			return (failedSimpleCmd(old, i));
	}
	else if (!ft_strcmp(redirOp, "<<"))
	{
		if (old->inFile.fd != STDIN_FILENO && old->inFile.fd)
			close(old->inFile.fd);
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_HEREDOC;
		old->inFile.fd = STDIN_FILENO;
	}
	else if (!ft_strcmp(redirOp, ">") || !ft_strcmp(redirOp, ">>"))
		handleOutRedir(old, redirOp, new);
	(*i)++;
	return (0);
}
