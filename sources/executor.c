/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 18:17:46 by edavid            #+#    #+#             */
/*   Updated: 2021/09/14 16:00:22 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** 
*/
int	executor(t_minishell *mystruct)
{
	(void)mystruct;
	return (0);
}
// pipex usage: ./pipex infile cmd1 cmd2 outfile
//				< infile cmd1 | cmd2 > outfile
//				./pipex infile cmd1 outfile
//				< infile cmd1 > outfile
// 				./pipex here_doc delimiter cmd1 cmd2 outfile
//				cmd1 << delimiter | cmd2 >> outfile
//				./pipex here_doc delimiter cmd1 outfile
//				cmd1 << delimiter >> outfile
// <outfile1 cat -e > outfile 
// EOF | wc -l