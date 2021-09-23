/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 12:30:04 by edavid            #+#    #+#             */
/*   Updated: 2021/09/23 12:45:50 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Finds the char '$' in 'str' and returns the pointer to it.
** Does not allocate, returns NULL if '$' was not matched.
** Takes quoting rules into consideration, ex. '$var' will not get expanded.
** But "$var" will.
*/
static char	*expandIndex(char *str)
{
	while (*str)
	{
		if (*str == '\'')
		{
			str = ft_strchr(str + 1, '\'');
			if (str == NULL)
				return (NULL);
		}
		else if (*str == '\"')
		{
			while (*++str && *str != '\"')
				if (*str == '$')
					return (str);
			if (!*str)
				return (NULL);
		}
		else if (*str == '$')
			return (str);
		str++;
	}
	return (NULL);
}

/*
** Allocates and returns a valid bash name starting from 'str'.
** Returns NULL if it cannot be a valid bash name.
** Currently supports: $?
*/
static char	*getParameter(const char *str)
{
	t_node_binary	*lst;
	char			*parameter;

	if (str == NULL || *str == '\0')
		return (NULL);
	if (*str == '?')
		return (ft_substr(str, 0, 1));
	if (*str != '_' && !ft_isalpha(*str))
		return (NULL);
	lst = NULL;
	ft_nodbinadd_front(&lst, ft_nodbinnew(ft_substr(str, 0, 1)));
	str++;
	while (*str)
	{
		if (*str != '_' && !ft_isalnum(*str))
			break ;
		ft_nodbinadd_front(&lst, ft_nodbinnew(ft_substr(str, 0, 1)));
		str++;
	}
	parameter = ft_nodbinstrjoin_from_back(lst);
	ft_nodbinclear(&lst, ft_nodbindel, -1);
	return (parameter);
}

/*
** Replaces 'param' at 'curTokenPtr[index]' with the corresponding
** Environment variable if it exists. If it doesnt exist, then it will simply
** be removed from 'curTokenPtr'.
*/
static void	expandStr(t_minishell *mystruct, char **curTokenPtr, char *param,
char *index)
{
	char		*start;
	char		*end;
	t_obj_lst	*ptr;

	start = ft_substr(*curTokenPtr, 0, index - *curTokenPtr);
	end = ft_strdup(index + ft_strlen(param) + 1);
	if (!ft_strcmp(param, "?"))
		start = ft_strjoin_free(start, ft_itoa((int)mystruct->fgExitStatus));
	else
	{
		ptr = ft_objlst_findbykey(mystruct->envpLst, param);
		if (ptr)
			start = ft_strjoin_free(start, ft_strdup(ptr->value));
	}
	start = ft_strjoin_free(start, end);
	free(*curTokenPtr);
	*curTokenPtr = start;
}

static void	replaceTokens(t_minishell *mystruct)
{
	ft_destroy_str_arr(&mystruct->tokens);
	mystruct->tokens = ft_nodbinstr_to_strarr(mystruct->tokensLst);
	ft_nodbinclear(&mystruct->tokensLst, ft_nodbindel, -1);
}

/*
** Performs the various shell expansions, breaking the expanded tokens into
** lists of filenames and commands and arguments.
** Currently supported: Environment variables ($ followed by some characters)
*/
int	expander(t_minishell *mystruct)
{
	int		i;
	char	*tmp;
	char	*parameter;

	i = -1;
	while (mystruct->tokens[++i])
	{
		tmp = expandIndex(mystruct->tokens[i]);
		while (tmp)
		{
			parameter = getParameter(tmp + 1);
			if (parameter == NULL)
				break ;
			expandStr(mystruct, &mystruct->tokens[i], parameter, tmp);
			free(parameter);
			tmp = expandIndex(mystruct->tokens[i]);
		}
		if (ft_strcmp(mystruct->tokens[i], ""))
			ft_nodbinadd_front(&mystruct->tokensLst,
				ft_nodbinnew(ft_strdup(mystruct->tokens[i])));
		if (isOperatorSyntaxErr(mystruct->tokens[i]))
			return (clearStruct(mystruct));
	}
	replaceTokens(mystruct);
	return (0);
}
