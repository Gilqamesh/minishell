/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_destroy_str_arr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 18:37:05 by edavid            #+#    #+#             */
/*   Updated: 2021/09/22 16:27:26 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

void	ft_destroy_str_arr(char ***str)
{
	int	i;

	if (str == NULL || *str == NULL)
		return ;
	i = -1;
	while ((*str)[++i])
		free((*str)[i]);
	free(*str);
	*str = NULL;
}
