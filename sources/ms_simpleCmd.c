/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_simpleCmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 10:50:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/20 19:10:00 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

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
