/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:06:11 by alex              #+#    #+#             */
/*   Updated: 2017/10/13 10:44:31 by aollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static int	is_g_base(void *ptr, size_t s)
{
	if (!g_base)
		return (EXIT_FAILLURE);
	if (!s)
	{
		free(ptr);
		return (EXIT_FAILLURE);
	}
	return (EXIT_SUCCESS);
}

/*
** Realoc function copy the ptr give in paramameter, in a new block
*/

void		*realloc(void *ptr, size_t s)
{
	t_block		*src;
	t_block		*dest;
	size_t		i;

	if (!ptr)
		return (malloc(s));
	if (is_g_base(ptr, s))
		return (NULL);
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
