/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/13 19:22:50 by edavid           ###   ########.fr       */
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
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_IN;
	}
	else if (!ft_strcmp(redirOp, ">"))
	{
		replaceStr(&old->outFile.filename, new);
		old->outFile.mode = REDIR_OUT;
	}
	else if (!ft_strcmp(redirOp, "<<"))
	{
		replaceStr(&old->inFile.filename, new);
		old->inFile.mode = REDIR_HEREDOC;
	}
	else if (!ft_strcmp(redirOp, ">>"))
	{
		replaceStr(&old->outFile.filename, new);
		old->outFile.mode = REDIR_APPEND;
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
