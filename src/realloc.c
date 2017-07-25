/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:06:11 by alex              #+#    #+#             */
/*   Updated: 2017/07/25 16:29:37 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Realoc function copy the ptr give in paramameter, in a new block
*/

void	*realloc(void *ptr, size_t s)
{
	t_block		*src;
	t_block		*dest;
	size_t		i;

	if (!ptr)
		return (malloc(s));
	if (!base)
		return (NULL);
	if (!s)
	{
		free(ptr);
		return (NULL);
	}
	src = ft_find_block(ft_find_zone_block(ptr), ptr);
	if (!src)
		return (NULL);
	if (src->size >= s)
		return (ptr);
	if (!(dest = malloc(s)))
		return (NULL);
	i = 0;
	while (i < s && i < src->size)
	{
		dest->data[i] = src->data[i];
		i++;
	}
	free(ptr);
	return (dest->data);
}
