/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quote_removal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 15:25:13 by edavid            #+#    #+#             */
/*   Updated: 2021/09/21 15:30:14 by edavid           ###   ########.fr       */
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
			ft_nodbinadd_front(charLst, ft_nodbinnew(ft_substr(prev + 1, 0,
						s - prev - 1)));
			len -= ++s - prev - 2;
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
char	*removeQuotes(char *s, size_t len)
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

int	quoteRemoval(t_minishell *mystruct)
{
	int		i;
	char	*tmp;

	i = -1;
	while (mystruct->tokens[++i])
	{
		tmp = removeQuotes(mystruct->tokens[i], ft_strlen(mystruct->tokens[i]));
		if (tmp == NULL)
		{
			clearStruct(mystruct);
			ft_putstr_fd("Something went wrong in quoteRemoval\n",
				STDERR_FILENO);
			return (1);
		}
		free(mystruct->tokens[i]);
		mystruct->tokens[i] = tmp;
	}
	return (0);
}
