/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 13:22:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/05 19:28:30 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "../mylib/mylib.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_minishell
{

}	t_minishell;

char	*myreadline(const char *promptMsg);
char	**convertStrToTokens(char *str);
void	parseCmdLine(t_minishell *mystruct, char *CmdLine);

#endif
