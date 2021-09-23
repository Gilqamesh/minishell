/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsiddiqu <gsiddiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 18:42:34 by edavid            #+#    #+#             */
/*   Updated: 2021/09/23 15:05:46 by gsiddiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

void	ft_ctlsigchars(t_minishell *mystruct, int flag)
{
	struct termios	termattr;

	//printf("Setting %d file descriptor attribute to %s sigchars.\n", STDIN_FILENO, flag ? "show":"don't show");
	termattr = mystruct->oattr;
	if (flag == 0)
		termattr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &termattr);
}

void	sighandler(int sig)
{
	if ((getMystruct(NULL))->lastPID != 0)
	{
		kill((getMystruct(NULL))->lastPID, sig);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	else if (sig == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	init_envp(t_minishell *mystruct, char **envp)
{
	char	*value;
	char	*key;
	int		i;

	i = -1;
	while (envp[++i])
	{
		value = ft_strchr(envp[i], '=');
		if (value == NULL)
			continue ;
		key = ft_substr(envp[i], 0, value - envp[i]);
		value = getenv(key);
		if (value)
		{
			ft_objlstadd_front(&mystruct->envpLst, ft_objlst_new(ft_strdup(key),
					ft_strdup(value)));
			ft_objlstinsert(&mystruct->exportedVars,
				ft_objlst_new(ft_strjoin_free(ft_strdup("declare -x "),
						ft_strdup(key)), ft_strjoin_free(ft_strdup("\""),
						ft_strjoin_free(ft_strdup(value), ft_strdup("\"")))));
		}
		free(key);
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
	tcgetattr(STDIN_FILENO, &(mystruct->oattr));
	init_envp(mystruct, envp);
}

/*
** Clears variables in 'mystruct' and prepares for next command line.
** Returns 1
*/
int	clearStruct(t_minishell *mystruct)
{
	t_list		*cur;

	if (mystruct->promptStr)
	{
		free(mystruct->promptStr);
		mystruct->promptStr = NULL;
	}
	ft_destroy_str_arr(&mystruct->tokens);
	ft_nodbinclear(&mystruct->tokensLst, ft_nodbindel, -1);
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
	return (1);
}
