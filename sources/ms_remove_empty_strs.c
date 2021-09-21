/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_remove_empty_strs.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:39:47 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 16:02:07 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

int	removeEmptyStrTokens(t_minishell *mystruct)
{
	t_node_binary	*tokens;
	int				i;

	tokens = NULL;
	i = -1;
	while (mystruct->tokens[++i])
	{
		if (ft_strcmp(mystruct->tokens[i], "") == 0)
			continue ;
		ft_nodbinadd_front(&tokens, ft_nodbinnew(
				ft_strdup(mystruct->tokens[i])));
	}
	ft_destroy_str_arr(&mystruct->tokens);
	mystruct->tokens = ft_nodbinstr_to_strarr(tokens);
	ft_nodbinclear(&tokens, ft_nodbindel, -1);
	return (0);
}
