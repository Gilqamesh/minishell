/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 17:45:56 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

void	sighandler(int sig)
{
	struct termios	termattr;

	if ((getMystruct(NULL))->lastPID != 0)
	{
		kill((getMystruct(NULL))->lastPID, sig);
	}
	else if (sig == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	tcgetattr(STDIN_FILENO, &termattr);
	termattr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &termattr);
}

static void	init_envp(t_minishell *mystruct, char **envp)
{
	char	*tmp;
	char	*var;
	int		i;

	i = -1;
	while (envp[++i])
	{
		tmp = ft_strchr(envp[i], '=');
		if (tmp == NULL)
			continue ;
		var = ft_substr(envp[i], 0, tmp - envp[i]);
		tmp = getenv(var);
		if (tmp)
			ft_objlstadd_front(&mystruct->envpLst, ft_objlst_new(ft_strdup(var),
					ft_strdup(tmp)));
		free(var);
	}
}

/*
** Initialize 'mystruct'
*/
void	init_mystruct(t_minishell *mystruct, char **envp)
{
	signal(SIGINT, &sighandler);
	signal(SIGQUIT, &sighandler);
	ft_bzero(mystruct, sizeof(*mystruct));
	init_envp(mystruct, envp);
}

/*
** Clears variables in 'mystruct' and prepares for next command line.
*/
void	clearStruct(t_minishell *mystruct)
{
	t_list		*cur;

	if (mystruct->promptStr)
	{
		free(mystruct->promptStr);
		mystruct->promptStr = NULL;
	}
	ft_destroy_str_arr(&mystruct->tokens);
	ft_simpleCmdclear(&mystruct->nodes, ft_simpleCmddel);
	cur = mystruct->pipeLines;
	while (cur)
	{
		ft_simpleCmdclear((t_simpleCmd **)&cur->content, ft_simpleCmddel);
		cur = cur->next;
	}
	ft_lstclear(&mystruct->pipeLines, ft_lstdel);
	ft_lstclear(&mystruct->allocedPointers, ft_lstdel);
	mystruct->lastPID = 0;
}
