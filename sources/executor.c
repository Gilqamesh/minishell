/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 18:17:46 by edavid            #+#    #+#             */
/*   Updated: 2021/09/16 18:15:36 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Builds char **argv for ft_pipex().
*/
static char	**buildArgv(t_simpleCmd *simpleCmd, t_std_FDs *FDs)
{
	int			strArrLen;
	char		**argv;
	t_simpleCmd	*cur;
	int			i;

	strArrLen = ft_simpleCmdsize(simpleCmd);
	argv = ft_calloc(strArrLen + 4, sizeof(*argv));
	argv[0] = ft_strdup("./pipex");
	if (FDs->inFile.filename)
		argv[1] = ft_strdup(FDs->inFile.filename);
	else
		argv[1] = ft_strdup("");
	if (FDs->outFile.filename)
		argv[strArrLen + 2] = ft_strdup(FDs->outFile.filename);
	else
		argv[strArrLen + 2] = ft_strdup("");
	cur = simpleCmd;
	i = 1;
	while (cur)
	{
		argv[++i] = ft_strArrtoStr(cur->arguments, ' ');
		cur = cur->next;
	}
	return (argv);
}

/*
** Executes the pipeline with ft_pipex() in 'mystruct'->pipeLines
*/
int	executor(t_minishell *mystruct)
{
	t_std_FDs	FDs;
	t_list		*cur;
	t_simpleCmd	*tmp;
	char		**argv;

	cur = mystruct->pipeLines;
	while (cur)
	{
		FDs = ((t_simpleCmd *)cur->content)->FDs;
		tmp = (t_simpleCmd *)cur->content;
		while (tmp->next)
			tmp = tmp->next;
		FDs.outFile = tmp->FDs.outFile;
		argv = buildArgv((t_simpleCmd *)cur->content, &FDs);
		printStrArr(argv);
		ft_pipex(mystruct, argv, &FDs);
		cur = cur->next;
	}
	return (0);
}
