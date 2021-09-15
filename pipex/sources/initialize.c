/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 15:50:33 by edavid            #+#    #+#             */
/*   Updated: 2021/09/15 19:34:19 by edavid           ###   ########.fr       */
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
static void	initialize_Cmds(t_pipex *mystruct, char *argv[])
{
	int			i;
	int			firstArgIndex;

	if (mystruct->isHereDoc)
		firstArgIndex = 3;
	else
		firstArgIndex = 2;
	i = -1;
	while (++i < mystruct->nOfCmds)
	{
		mystruct->commands[i] = ft_split(argv[i + firstArgIndex], ' ');
		if (mystruct->commands[i] == NULL)
			error_handler(mystruct, PIPEX_ERR, "ft_split() failed\n");
		if (mystruct->commands[i][0] == NULL)
			error_handler(mystruct, PIPEX_EUSAGE, "Empty command\n");
		cmd_path(&mystruct->commands[i][0], mystruct->envpLst);
	}
}

/*
** Initializes values related to here_doc in 'mystruct'
*/
static void	init_hereDoc(t_pipex *mystruct, int argc, char **argv)
{
	mystruct->isHereDoc = true;
	mystruct->nOfCmds--;
	mystruct->file[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND,
		0777);
	if (mystruct->file[1] == -1)
		error_handler(mystruct, PIPEX_EFOPEN, "Could not open outfile\n");
	if (mystruct->nOfCmds < 1)
		error_handler(mystruct, PIPEX_EUSAGE,
			"Usage: ./pipex here_doc LIMITER cmd [commands ...] outfile\n");
	mystruct->delimiter = ft_strdup(argv[2]);
	if (mystruct->delimiter == NULL)
		error_handler(mystruct, PIPEX_ERR, "Malloc failed\n");
	ft_lstadd_front(&mystruct->alloced_lst, ft_lstnew(mystruct->delimiter));
}

/*
** Initializes t_pipex variable
*/
void	initialize_mystruct(t_pipex *mystruct, t_std_FDs *FDs)
{
	mystruct->nOfCmds = mystruct->argc - 3;
	if (mystruct->nOfCmds < 1)
		error_handler(mystruct, PIPEX_EUSAGE,
			"Usage: ./pipex infile cmd1 [additional commands ...] outfile\n");
	if (!ft_strcmp(mystruct->argv[1], "here_doc"))
		init_hereDoc(mystruct, mystruct->argc, mystruct->argv);
	else if (ft_strcmp(mystruct->argv[1], ""))
		initOutFile(mystruct, mystruct->argc, mystruct->argv, FDs);
	mystruct->commands = ft_lstmallocwrapper(&mystruct->alloced_lst,
			mystruct->nOfCmds * sizeof(*mystruct->commands), true);
	if (mystruct->commands == NULL)
		error_handler(mystruct, PIPEX_EMALLOC, "Malloc failed\n");
	initialize_Cmds(mystruct, mystruct->argv);
	mystruct->pipes = ft_lstmallocwrapper(&mystruct->alloced_lst,
			mystruct->nOfCmds * sizeof(*mystruct->pipes), false);
	if (mystruct->pipes == NULL)
		error_handler(mystruct, PIPEX_EMALLOC, "Malloc failed\n");
	mystruct->openPipes = ft_lstmallocwrapper(&mystruct->alloced_lst,
			mystruct->nOfCmds * sizeof(*mystruct->openPipes), true);
	if (mystruct->openPipes == NULL)
		error_handler(mystruct, PIPEX_EMALLOC, "Malloc failed\n");
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
