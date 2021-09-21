/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:59 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 15:23:04 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

static int	addOperatorTokens(t_node_binary **tokenlst, char **str, char **prev)
{
	while (isCharPartOfSet(**str, METACHARACTERS))
	{
		*str += ft_trim_from_left_index(*str, WHITESPACES);
		if (isCharPartOfSet(**str, OPERATORS))
		{
			*prev = *str;
			*str += ft_trim_from_left_index(*str, OPERATORS);
			ft_nodbinadd_front(tokenlst, ft_nodbinnew(ft_substr(*prev, 0,
						*str - *prev)));
		}
		*str += ft_trim_from_left_index(*str, WHITESPACES);
	}
	if (!**str)
		return (1);
	return (0);
}

/*
** Breaks down 'str' into allocated tokens and appends 'tokenlst' with them.
*/
static int	appendLstWithTokens(t_minishell *mystruct, t_node_binary **tokenlst)
{
	char		*prev;
	char		*str;

	str = mystruct->promptStr;
	while (*str)
	{
		if (addOperatorTokens(tokenlst, &str, &prev))
			break ;
		prev = str;
		while (*str && !isCharPartOfSet(*str, METACHARACTERS))
		{
			if (*str == '\'')
				str = ft_strchr(str + 1, '\'');
			else if (*str == '\"')
				str = ft_strchr(str + 1, '\"');
			if (str++ == NULL)
			{
				clearStruct(mystruct);
				return (1);
			}
		}
		ft_nodbinadd_front(tokenlst,
			ft_nodbinnew(ft_substr(prev, 0, str - prev)));
	}
	return (0);
}

/*
** Allocates 'mystruct->tokens' with a NULL terminated array of strings.
** The strings are separated by metacharacters, obeying the quoting rules.
*/
int	lexer(t_minishell *mystruct)
{
	t_node_binary	*tokenlst;

	tokenlst = NULL;
	if (appendLstWithTokens(mystruct, &tokenlst))
		return (1);
	mystruct->tokens = ft_nodbinstr_to_strarr(tokenlst);
	ft_nodbinclear(&tokenlst, ft_nodbindel, -1);
	return (0);
}
