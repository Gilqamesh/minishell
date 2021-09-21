/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 13:55:05 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static int	dealWithNextCmd(t_simpleCmd **cur)
{
	*cur = (*cur)->next;
	if (*cur && (*cur)->FDs.inFile.mode == REDIR_NONE)
		(*cur)->FDs.inFile.mode = REDIR_VOID;
	return (1);
}

/*
** Builds mystruct->pipeLines that is a linked list of t_simpleCmd linked lists.
** Simple command:
** [< filename]* command [< filename]*
** the arguments for the commandcan be anywhere after the command
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
			if (cur->FDs.outFile.filename && dealWithNextCmd(&cur))
				break ;
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
