/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/19 21:45:14 by edavid           ###   ########.fr       */
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
		if (old->inFile.fd != STDIN_FILENO && old->inFile.fd)
			close(old->inFile.fd);
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_IN;
		old->inFile.fd = open(old->inFile.filename, O_RDONLY);
	}
	else if (!ft_strcmp(redirOp, ">"))
	{
		if (old->outFile.fd != STDOUT_FILENO && old->outFile.fd)
			close(old->outFile.fd);
		replaceStr(&old->outFile.filename, new);
		old->outFile.mode = REDIR_OUT;
		old->outFile.fd = open(old->outFile.filename, O_WRONLY | O_CREAT
			| O_TRUNC, 0777);
	}
	else if (!ft_strcmp(redirOp, "<<"))
	{
		if (old->inFile.fd != STDIN_FILENO && old->inFile.fd)
			close(old->inFile.fd);
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_HEREDOC;
		old->inFile.fd = STDIN_FILENO;
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

/*
** Initializes and adds to mystruct->nodes;
** Multiple here_doc is not supported like in bash.
*/
static void	readSimpleCommand(t_minishell *mystruct, int *i)
{
	t_std_FDs		FD;
	t_node_binary	*tmpLst;
	bool			isBuiltin;

	initFD(&FD);
	tmpLst = NULL;
	isBuiltin = false;
	while (mystruct->tokens[*i] && ft_strcmp(mystruct->tokens[*i], "|"))
	{
		if (isValidRedirection(mystruct->tokens[*i]))
		{
			replaceFD(&FD, mystruct->tokens[*i], mystruct->tokens[*i + 1]);
			(*i)++;
		}
		else
		{
			if (tmpLst == NULL && isStrBuiltin(mystruct->tokens[*i]))
				isBuiltin = true;
			ft_nodbinadd_front(&tmpLst, ft_nodbinnew(ft_strdup(
				mystruct->tokens[*i])));
		}
		(*i)++;
	}
	if (mystruct->tokens[*i])
		(*i)++;
	ft_simpleCmdadd_back(&mystruct->nodes,
		ft_simpleCmdnew(ft_nodbinstr_to_strarr(tmpLst), &FD, isBuiltin));
	ft_nodbinclear(&tmpLst, ft_nodbindel, -1);
}

// Simple command:
// [< filename]* command [< filename]*
// the arguments for the commandcan be anywhere after the command

/*
** Builds mystruct->pipeLines that is a linked list of t_simpleCmd linked lists.
*/
static void	buildPipeline(t_minishell *mystruct)
{
	t_simpleCmd	*cur;
	t_simpleCmd	*tmp;
	t_simpleCmd	*new;

	cur = mystruct->nodes;
	while (cur)
	{
		tmp = NULL;
		while (cur)
		{
			new = ft_simpleCmdnew(ft_strArrDup(cur->arguments),
				copy_FD(&cur->FDs), false);
			ft_simpleCmdadd_back(&tmp, new);
			if (cur->FDs.outFile.filename)
			{
				cur = cur->next;
				if (cur && cur->FDs.inFile.mode == REDIR_NONE)
					cur->FDs.inFile.mode = REDIR_VOID;
				break ;
			}
			cur = cur->next;
			if (cur && cur->FDs.inFile.filename)
			{
				new->FDs.outFile.mode = REDIR_VOID;
				break ;
			}
		}
		ft_lstadd_back(&mystruct->pipeLines, ft_lstnew(tmp));
	}
}

/*
** Parses mystruct->tokens and adds to mystruct->nodes each simple command,
** that are separated with pipes.
** Then builds mystruct->pipeLines from mystruct->nodes.
*/
int	parser(t_minishell *mystruct)
{
	int	i;

	i = 0;
	while (mystruct->tokens[i])
		readSimpleCommand(mystruct, &i);
	buildPipeline(mystruct);
	return (0);
}
// cmd1 >file | cmd2
// cmd2's input is EOF
// cmd1 | cmd2
// cmd1's input is EOF
// cmd1 | cmd2 <file
// Pipeline:
// If next command has input redirection 		-> end pipeline with current command
// If current command has output redirection 	-> end pipeline with current command
//
// If first command: 		if has no input redirection, input is EOF