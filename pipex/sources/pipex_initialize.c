/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 15:50:33 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 19:53:12 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/ft_minishell.h"

/*
** Initializes command arguments and store them
** Not supported: 'awk'
** Warning, delimiter used for each argument is ' ', so stuff like awk is not
** handled, ex.: "awk '{print $3}'" would be interpreted as:
** "awk" "'{print" "$3}'"
*/
static int	initialize_Cmds(t_pipex *mystruct, t_simpleCmd *pipeLine)
{
	t_simpleCmd	*cur;

	cur = pipeLine;
	while (cur)
	{
		if (cur->isBuiltin == false)
			cmd_path(&cur->arguments[0], mystruct->envpLst);
		cur = cur->next;
	}
	return (0);
}

/*
** Initializes values related to here_doc in 'mystruct'
*/
static int	init_hereDoc(t_pipex *mystruct)
{
	mystruct->delimiter = ft_strdup(mystruct->first->FDs.inFile.filename);
	if (mystruct->delimiter == NULL)
		return (terminate_pipex(mystruct, "Malloc failed\n"));
	ft_lstadd_front(&mystruct->alloced_lst, ft_lstnew(mystruct->delimiter));
	return (0);
}

/*
** Initializes t_pipex variable
*/
int	initialize_mystruct(t_minishell *minishellStruct, t_pipex *mystruct,
t_simpleCmd *pipeLine)
{
	char	*tmp;

	ft_bzero(mystruct, sizeof(*mystruct));
	tmp = ft_strjoin_free(ft_strdup("PATH="),
			ft_strdup(ft_objlst_findbykey(minishellStruct->envpLst,
					"PATH")->value));
	mystruct->envp = ft_strToStrArr(tmp);
	free(tmp);
	mystruct->envpLst = minishellStruct->envpLst;
	mystruct->first = pipeLine;
	mystruct->last = pipeLine;
	while (mystruct->last->next)
		mystruct->last = mystruct->last->next;
	mystruct->nOfCmds = ft_simpleCmdsize(pipeLine);
	if (!mystruct->nOfCmds)
		return (terminate_pipex(mystruct,
				"Usage: [simpleCmd1] [simpleCmd2] ...\n"));
	if (pipeLine->FDs.inFile.mode == REDIR_HEREDOC
		&& init_hereDoc(mystruct))
		return (1);
	if (initialize_Cmds(mystruct, pipeLine))
		return (1);
	if (pipex_init_pipes(mystruct))
		return (terminate_pipex(mystruct, "Malloc failed\n"));
	return (0);
}

/*
** Returns allocated string, that is the path for 'cmd'
** It tries out all the possibilities in the string array 'paths'
** If none of them are valid paths for 'cmd', the function returns NULL
*/
static char	*get_cur_path(char **paths, char **cmd)
{
	int		i;
	char	*cur_path;

	i = -1;
	while (paths[++i])
	{
		cur_path = ft_strjoin_free(ft_strdup(paths[i]), ft_strjoin("/", *cmd));
		if (!access(cur_path, F_OK | X_OK))
			break ;
		free(cur_path);
	}
	if (paths[i] == NULL)
		cur_path = NULL;
	return (cur_path);
}

/*
** Allocates and returns a string that is the path of the UNIX command 'cmd'
** 'envp' is the environment variable from the shell
*/
void	cmd_path(char **cmd, t_obj_lst *envp)
{
	char		**paths;
	char		*cur_path;
	t_obj_lst	*keyPtr;

	keyPtr = ft_objlst_findbykey(envp, "PATH");
	if (keyPtr == NULL)
		return ;
	paths = ft_split(keyPtr->value, ':');
	cur_path = get_cur_path(paths, cmd);
	ft_destroy_str_arr(&paths);
	if (cur_path == NULL && !access(*cmd, F_OK | X_OK))
		return ;
	free(*cmd);
	*cmd = cur_path;
}
