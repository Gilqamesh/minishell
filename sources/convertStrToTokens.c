/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convertStrToTokens.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:59 by edavid            #+#    #+#             */
/*   Updated: 2021/09/07 18:11:31 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Returns true if 'c' is part of 'set', otherwise false.
*/
static bool	isCharPartOfSet(char c, char *set)
{
	if (set == NULL)
		return (false);
	while (*set)
		if (c == *set++)
			return (true);
	return (false);
}

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
			ft_nodbinadd_front(charLst, ft_nodbinnew(ft_substr(prev + 1, 0,
				s - prev - 1)));
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
static void	appendLstWithTokens(t_node_binary **tokenlst, char *str)
{
	char		*prev;

	while (*str)
	{
		while (isCharPartOfSet(*str, METACHARACTERS))
			str++;
		prev = str;
		if (!*str)
			break ;
		while (*str && !isCharPartOfSet(*str, METACHARACTERS))
		{
			if (*str == '\'')
				str = ft_strchr(str + 1, '\'');
			else if (*str == '\"')
				str = ft_strchr(str + 1, '\"');
			if (str == NULL)
				// This can't happen, problem with 'isValidCmdLine' function.
				// This exit() call is for debugging only, remove it later
				// As we never want to exit from the shell.
				exit(1);
			str++;
		}
		ft_nodbinadd_front(tokenlst,
			ft_nodbinnew(removeQuotes(prev, str - prev)));
	}
}

/*
** Allocates and returns a NULL terminated array of strings.
** The strings are separated by metacharacters, obeying the quoting rules.
*/
char	**convertStrToTokens(char *str)
{
	char			**tokenarr;
	t_node_binary	*tokenlst;

	tokenlst = NULL;
	appendLstWithTokens(&tokenlst, str);
	tokenarr = ft_nodbinstr_to_strarr(tokenlst);
	ft_nodbinclear(&tokenlst, ft_nodbindel, -1);
	return (tokenarr);
}
