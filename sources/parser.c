/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:23:57 by edavid            #+#    #+#             */
/*   Updated: 2021/09/08 16:43:26 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Checks if 'token' is a valid "word" in bash or
** If 'token' is a valid operator (Thats actually not as easy..)
** For now just check if 'token' is a valid metacharacter or ">>" "<<"
*/
static bool	isValidToken(char *token)
{
	if (!ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"))
		return (true);
	if (ft_strlen(token) == 1)
	{
		if (isCharPartOfSet(*token, OPERATORS))
			return (true);
	}
	return (false);
}

/*
** Parses the tokens and builds the 'nodes' list in 'mystruct' using 't_node'
*/
void	parser(t_minishell *mystruct, char **tokens)
{
	int		i;

	i = -1;
	while (tokens[++i])
	{
		if (isValidToken(tokens[i]) == false)
		{
			ft_printf("syntax error near unexpected token `%s'\n", tokens[i]);
			return ;
		}
	}
	(void)mystruct;
	return ;
}
