/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsiddiqu <gsiddiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 15:35:52 by gsiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

void sighandler(int sig)
{
	struct termios termattr;

	if ((getMystruct(NULL))->lastPID != 0)
	{
		kill((getMystruct(NULL))->lastPID, sig);
	}
	else if (sig == SIGINT)
	{
		*rl_line_buffer = '\0';
		rl_point = 0;
		rl_end = 0;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_redisplay();
		ft_putstr_fd(CMD_PROMPT, STDOUT_FILENO);
	}
	tcgetattr(STDIN_FILENO, &termattr);
	termattr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &termattr);
}

static void	init_envp(t_minishell *mystruct)
{
	char	*path;

	path = getenv("PATH");
	if (path)
		ft_objlstadd_front(&mystruct->envpLst, ft_objlst_new(ft_strdup("PATH"),
			ft_strdup(path)));
	path = getenv("PWD");
	if (path)
	{
		ft_objlstadd_front(&mystruct->envpLst,
			ft_objlst_new(ft_strdup("PWD"), ft_strdup(path)));
		ft_objlstadd_front(&mystruct->envpLst,
			ft_objlst_new(ft_strdup("OLDPWD"), ft_strdup(path)));
	}
}

/*
** Initialize 'mystruct'
*/
void	init_mystruct(t_minishell *mystruct)
{
	signal(SIGINT, &sighandler);
	signal(SIGQUIT, &sighandler);
	ft_bzero(mystruct, sizeof(*mystruct));
	init_envp(mystruct);
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
