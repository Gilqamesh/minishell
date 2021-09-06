/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 15:46:59 by edavid            #+#    #+#             */
/*   Updated: 2021/09/06 16:17:55 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/ft_minishell.h"

bool	isStackEmpty(t_stack *sp)
{
	if (sp->size == 0)
		return (true);
	return (false);
}

void	pushStack(t_stack *sp, void *content)
{
	ft_lstadd_front(&sp->stack, ft_lstnew(content));
	sp->size++;
}

void	destroyStack(t_stack *sp)
{
	ft_lstclear(&sp->stack, ft_lstdel);
	sp->size = 0;
}

void	*popStack(t_stack *sp)
{
	void	*content;
	t_list	*tmp;

	if (isStackEmpty(sp) == true)
		return (NULL);
	content = sp->stack->content;
	tmp = sp->stack;
	sp->stack = sp->stack->next;
	free(tmp);
	sp->size--;
	return (content);
}

t_stack	*createStack(void)
{
	t_stack	*sp;

	sp = malloc(sizeof(*sp));
	sp->size = 0;
	sp->stack = NULL;
	return (sp);
}
