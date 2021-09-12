/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 18:17:46 by edavid            #+#    #+#             */
/*   Updated: 2021/09/12 21:15:58 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** pipex general usage:
** < infile cmd1 > outfile
** input for cmd1: infile
** output for cmd1: outfile
** <command> | <command> | <command> ...
** <command	::=	<simple command>
** 	        |	<simple command> '<' <filename>
** 	        |	<simple command> '>' <filename>
** < infile cmd1 | cmd2 | cmd3 | cmd4 > outfile
** each <command> is a potential call of pipex
*/
int	executor(t_minishell *mystruct)
{
	(void)mystruct;
	return (0);
}

// Observations
//
// echo "hey" | wc -l
// echo "hey" | wc -l outfile3     both are inputs for wc -l, first the output of the pipeline
//									and then the arguments of wc
// wc -l < outfile 3     input redirection to wc
// wc -l < outfile3 outfile3  '< outfile3' input redirection will get replaced by the arguments that follow
// echo "hey" | wc -l < outfile3 outfile3   both the input from echo and '< outfile3' is replaced
// GENERALLY?
// echo "hey" | wc -l < outfile3 === ./pipex IN "echo hey" "wc -l" OUT  + ./pipex "outfile3" "wc -l" OUT
// echo "hey" | cat < outfile3 outfile === cat outfile
// echo "hey" | cat < outfile3 outfile outfile === cat outfile outfile
// echo "hey" | echo "hi" | cat < outfile3