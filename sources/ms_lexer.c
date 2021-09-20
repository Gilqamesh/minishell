/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:59 by edavid            #+#    #+#             */
/*   Updated: 2021/09/12 14:12:55 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Breaks down 's' into substrings, removing matching ' or " from the string.
** Appends 'charLst' with these substrings.
** Only checks up to 'len' in 's'.
*/
static void	appendLstWithChars(t_node_binary **charLst, char *s, size_t len)
{
	char	*prev;

	while (*s && len)
	{
		if (*s == '\'' || *s == '\"')
		{
			prev = s;
			if (*s == '\'')
				s = ft_strchr(s + 1, '\'');
			else if (*s == '\"')
				s = ft_strchr(s + 1, '\"');
			if (s == NULL)
			{
				ft_nodbinclear(charLst, ft_nodbindel, -1);
				return ;
			}
			ft_nodbinadd_front(charLst, ft_nodbinnew(ft_substr(prev, 0,
				s - prev + 1)));
			len -= s++ - prev;
			continue ;
		}
		ft_nodbinadd_front(charLst, ft_nodbinnew(ft_substr(s++, 0, 1)));
		len--;
	}
}

/*
** Behaves like the 'ft_substr' function, but removes pairs of quotes
** example: removeQuotes("ab'cd'ef", 7) === "abcde"
*/
static char	*removeQuotes(char *s, size_t len)
{
	t_node_binary	*charLst;
	char			*newStr;

	if (s == NULL)
		return (NULL);
	charLst = NULL;
	appendLstWithChars(&charLst, s, len);
	newStr = ft_nodbinstrjoin_from_back(charLst);
	ft_nodbinclear(&charLst, ft_nodbindel, -1);
	return (newStr);
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
		while (isCharPartOfSet(*str, METACHARACTERS))
		{
			str += ft_trim_from_left_index(str, WHITESPACES);
			if (isCharPartOfSet(*str, OPERATORS))
			{
				prev = str;
				str += ft_trim_from_left_index(str, OPERATORS);
				ft_nodbinadd_front(tokenlst, ft_nodbinnew(ft_substr(prev, 0,
					str - prev)));
			}
			str += ft_trim_from_left_index(str, WHITESPACES);
		}
		if (!*str)
			break ;
		prev = str;
		while (*str && !isCharPartOfSet(*str, METACHARACTERS))
		{
			if (*str == '\'')
				str = ft_strchr(str + 1, '\'');
			else if (*str == '\"')
				str = ft_strchr(str + 1, '\"');
			if (str == NULL)
			{
				clearStruct(mystruct);
				return (1);
			}
			str++;
		}
		ft_nodbinadd_front(tokenlst,
			ft_nodbinnew(removeQuotes(prev, str - prev)));
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
