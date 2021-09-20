/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_simpleCmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 19:09:32 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 20:15:32 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Adds the element 'new' at the end of 'lst'.
*/
void	ft_simpleCmdadd_back(t_simpleCmd **lst, t_simpleCmd *new)
{
	t_simpleCmd	*tmp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/*
** Returns the size of 'list'.
*/
int	ft_simpleCmdsize(t_simpleCmd *list)
{
	int	len;

	if (list == NULL)
		return (0);
	len = 0;
	while (list)
	{
		list = list->next;
		len++;
	}
	return (len);
}

/*
** Allocates, initializes with 'arguments and 'FDs' and returns 't_simpleCmd *'.
*/
t_simpleCmd	*ft_simpleCmdnew(char **arguments, t_std_FDs FDs, bool isBuiltin)
{
	t_simpleCmd	*new;

	new = ft_calloc(1, sizeof(*new));
	new->arguments = arguments;
	new->FDs = FDs;
	new->isBuiltin = isBuiltin;
	return (new);
}
