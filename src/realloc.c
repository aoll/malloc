/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:06:11 by alex              #+#    #+#             */
/*   Updated: 2017/07/24 15:06:45 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Realoc function copy the ptr give in paramameter, in a new block
*/

void	*ft_realloc(void *ptr, size_t s)
{
	t_block *src;
	t_block *dest;
	void	*zone;
	int		i;

	if (!ptr)
		return (ft_malloc(s));
	if (!s)
	{
		ft_free(ptr);
		return (NULL);
	}
	zone = ft_find_zone_block(ptr);
	src = ft_find_block(zone, ptr);
	if (!src)
		return (ptr);
	dest = ft_malloc(s);
	while (i < s && i < src->size)
	{
		dest->data[i] = src->data[i];
		i++;
	}
	return (dest->data);
}
