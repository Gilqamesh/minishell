/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_read_simple_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 13:54:20 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 19:10:44 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static void	clearFD(t_std_FDs *FD)
{
	if (FD->inFile.fd != STDIN_FILENO)
		close(FD->inFile.fd);
	if (FD->outFile.fd != STDOUT_FILENO)
		close(FD->outFile.fd);
	if (FD->errFile.fd != STDERR_FILENO)
		close(FD->errFile.fd);
	if (FD->inFile.filename)
		free(FD->inFile.filename);
	if (FD->outFile.filename)
		free(FD->outFile.filename);
	if (FD->errFile.filename)
		free(FD->errFile.filename);
	ft_bzero(FD, sizeof(*FD));
}

static void	initSimpleCmdVars(t_std_FDs *FD, t_node_binary **tmpLst,
bool *isBuiltin)
{
	initFD(FD);
	*tmpLst = NULL;
	*isBuiltin = false;
}

static int	checkFD(t_node_binary **tmpLst, t_std_FDs *old, char **tokenArr,
int *i)
{
	if (replaceFD(old, tokenArr[*i], tokenArr[*i + 1], i))
	{
		clearFD(old);
		ft_nodbinclear(tmpLst, ft_nodbindel, -1);
		while (tokenArr[*i]
			&& ft_strcmp(tokenArr[*i], "|"))
			(*i)++;
		return (1);
	}
	return (0);
}

static void	addTokenToLst(t_node_binary **tmpLst, char *token, bool *isBuiltin)
{
	if (*tmpLst == NULL && isStrBuiltin(token))
		*isBuiltin = true;
	ft_nodbinadd_front(tmpLst, ft_nodbinnew(ft_strdup(token)));
}

/*
** Initializes and adds to mystruct->nodes;
** Multiple here_doc is not supported like in bash.
*/
void	readSimpleCommand(t_minishell *mystruct, int *i)
{
	t_std_FDs		FD;
	t_node_binary	*tmpLst;
	bool			isBuiltin;
	bool			ignoreSimpleCmd;

	ignoreSimpleCmd = false;
	initSimpleCmdVars(&FD, &tmpLst, &isBuiltin);
	while (mystruct->tokens[*i] && ft_strcmp(mystruct->tokens[*i], "|"))
	{
		if (isValidRedirection(mystruct->tokens[*i])
			&& checkFD(&tmpLst, &FD, mystruct->tokens, i))
		{
			ignoreSimpleCmd = true;
			break ;
		}
		else
			addTokenToLst(&tmpLst, mystruct->tokens[*i], &isBuiltin);
		(*i)++;
	}
	if (mystruct->tokens[*i])
		(*i)++;
	ft_simpleCmdadd_back(&mystruct->nodes,
		ft_simpleCmdnew(ft_nodbinstr_to_strarr(tmpLst), FD, isBuiltin,
			ignoreSimpleCmd));
	ft_nodbinclear(&tmpLst, ft_nodbindel, -1);
}
