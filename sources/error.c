/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 13:05:01 by edavid            #+#    #+#             */
/*   Updated: 2021/09/09 19:26:08 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Free all memory associated with 'mystruct'.
** Prints an error message to STDERR.
*/
void	free_mystruct(t_minishell *mystruct)
{
	ft_destroy_str_arr(&mystruct->tokens);
	ft_nodbinclear(&mystruct->envpLst, ft_nodbindel, -1);
	ft_destroy_str_arr(&mystruct->envp);
	ft_putstr_fd("Something went wrong\n", STDERR_FILENO);
}
