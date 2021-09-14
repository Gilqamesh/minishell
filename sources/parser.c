/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/14 13:13:17 by edavid           ###   ########.fr       */
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

static void	replaceFD(t_std_FDs *old, char *redirOp, char *new)
{
	if (!ft_strcmp(redirOp, "<"))
	{
		if (old->inFile.filename && old->inFile.fd != STDIN_FILENO)
			close(old->inFile.fd);
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_IN;
		old->inFile.fd = open(old->inFile.filename, O_RDONLY);
	}
	else if (!ft_strcmp(redirOp, ">"))
	{
		if (old->outFile.filename && old->outFile.fd != STDOUT_FILENO)
			close(old->outFile.fd);
		replaceStr(&old->outFile.filename, new);
		old->outFile.mode = REDIR_OUT;
		old->outFile.fd = open(old->outFile.filename, O_WRONLY | O_CREAT
			| O_TRUNC, 0777);
	}
	else if (!ft_strcmp(redirOp, "<<"))
	{
		if (old->inFile.filename && old->inFile.fd != STDIN_FILENO)
			close(old->inFile.fd);
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_HEREDOC;
		old->inFile.fd = STDIN_FILENO;
	}
	else if (!ft_strcmp(redirOp, ">>"))
	{
		if (old->outFile.filename && old->outFile.fd != STDOUT_FILENO)
		replaceStr(&old->outFile.filename, new);
		old->outFile.mode = REDIR_APPEND;
		old->outFile.fd = open(old->outFile.filename, O_CREAT | O_APPEND, 0777);
	}
}

/*
** Initializes and adds to mystruct->nodes;
** Multiple here_doc is not supported like in bash.
*/
static void	readSimpleCommand(t_minishell *mystruct, int *i)
{
	t_std_FDs		FD;
	t_node_binary	*tmpLst;

	ft_bzero(&FD, sizeof(FD));
	FD.inFile.fd = STDIN_FILENO;
	FD.outFile.fd = STDOUT_FILENO;
	FD.errFile.fd = STDERR_FILENO;
	tmpLst = NULL;
	while (mystruct->tokens[*i] && ft_strcmp(mystruct->tokens[*i], "|"))
	{
		if (isValidRedirection(mystruct->tokens[*i]))
		{
			replaceFD(&FD, mystruct->tokens[*i], mystruct->tokens[*i + 1]);
			(*i)++;
		}
		else
			ft_nodbinadd_front(&tmpLst, ft_nodbinnew(ft_strdup(
				mystruct->tokens[*i])));
		(*i)++;
	}
	if (mystruct->tokens[*i])
		(*i)++;
	ft_simpleCmdadd_back(&mystruct->nodes,
		ft_simpleCmdnew(ft_nodbinstr_to_strarr(tmpLst), &FD));
	ft_nodbinclear(&tmpLst, ft_nodbindel, -1);
}

// Simple command:
// [< filename]* command [< filename]*
// the arguments for the commandcan be anywhere after the command

/* 
** Parses mystruct->tokens and adds to mystruct->nodes each simple command,
** that are separated with pipes.
*/
int	parser(t_minishell *mystruct)
{
	int	i;

	i = 0;
	while (mystruct->tokens[i])
		readSimpleCommand(mystruct, &i);
	return (0);
}
