/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_shell_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 10:50:09 by edavid            #+#    #+#             */
/*   Updated: 2021/09/10 11:25:02 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

/*
** Adds the element 'new' at the end of 'lst'.
*/
void	ft_shell_node_add_back(t_shell_node **lst, t_shell_node *new)
{
	t_shell_node	*tmp;

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
void	ft_shell_node_clear(t_shell_node **lst, void (*del)(void *))
{
	t_shell_node	*cur;
	t_shell_node	*tmp;

	if (lst == NULL)
		return ;
	cur = *lst;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		ft_shell_node_delone(tmp, del);
	}
	*lst = NULL;
}

/*
** Takes as a parameter an element and frees the memory of the element's
** content using the function 'del' given as a parameter and free the element.
** The memory of 'next' is not freed.
*/
void	ft_shell_node_delone(t_shell_node *item, void (*del)(void *))
{
	(*del)(item);
}

/*
** Frees all memory in the structure pointed to by 'item'.
** Frees 'item'.
*/
void	ft_shell_node_del(void *item)
{
	if (((t_shell_node *)item)->type == TYPE_OPERATOR)
		free(((t_shell_node *)item)->u_data.operator);
	else if (((t_shell_node *)item)->type == TYPE_FILENAME)
		free(((t_shell_node *)item)->u_data.filename);
	else if (((t_shell_node *)item)->type == TYPE_COMMAND)
		ft_destroy_str_arr(&((t_shell_node *)item)->u_data.arguments);
	if (((t_shell_node *)item)->FDs.inFile)
		free(((t_shell_node *)item)->FDs.inFile);
	if (((t_shell_node *)item)->FDs.outFile)
		free(((t_shell_node *)item)->FDs.outFile);
	if (((t_shell_node *)item)->FDs.errFile)
		free(((t_shell_node *)item)->FDs.errFile);
}
