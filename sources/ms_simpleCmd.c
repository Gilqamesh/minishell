/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_simpleCmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 10:50:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/18 13:07:56 by edavid           ###   ########.fr       */
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
** Deletes and frees the given element and every successor of that element
** using the function 'del' and free. Finally, the pointer to the list is
** set to NULL.
*/
void	ft_simpleCmdclear(t_simpleCmd **lst, void (*del)(void *))
{
	t_simpleCmd	*cur;
	t_simpleCmd	*tmp;

	if (lst == NULL)
		return ;
	cur = *lst;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		ft_simpleCmddelone(tmp, del);
	}
	*lst = NULL;
}

/*
** Takes as a parameter an element and frees the memory of the element's
** content using the function 'del' given as a parameter and free the element.
** The memory of 'next' is not freed.
*/
void	ft_simpleCmddelone(t_simpleCmd *item, void (*del)(void *))
{
	(*del)(item);
}

/*
** Frees all memory in the structure pointed to by 'item'.
** Frees 'item'.
*/
void	ft_simpleCmddel(void *item)
{
	if (item == NULL)
		return ;
	if (((t_simpleCmd *)item)->arguments)
		ft_destroy_str_arr(&((t_simpleCmd *)item)->arguments);
	if (((t_simpleCmd *)item)->FDs.inFile.filename)
		free(((t_simpleCmd *)item)->FDs.inFile.filename);
	if (((t_simpleCmd *)item)->FDs.outFile.filename)
		free(((t_simpleCmd *)item)->FDs.outFile.filename);
	if (((t_simpleCmd *)item)->FDs.errFile.filename)
		free(((t_simpleCmd *)item)->FDs.errFile.filename);
	free(item);
}

/*
** Allocates, initializes with 'arguments and 'FDs' and returns 't_simpleCmd *'.
*/
t_simpleCmd	*ft_simpleCmdnew(char **arguments, t_std_FDs *FDs, bool isBuiltin)
{
	t_simpleCmd	*new;

	new = malloc(sizeof(*new));
	new->arguments = arguments;
	new->FDs = *FDs;
	new->isBuiltin = isBuiltin;
	new->next = NULL;
	return (new);
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
