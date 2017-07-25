/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 14:24:37 by alex              #+#    #+#             */
/*   Updated: 2017/07/25 16:16:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Free a large zone in memory
*/

static void		ft_free_large(t_block *b)
{
	t_zone *zone;

	zone = base;
	if (!base)
		return ;
	if (b->prev)
		b->prev->next = b->next;
	if (!b->prev)
		zone->large = b->next;
	if (b->next)
		b->next->prev = b->prev;
	munmap(b, b->size + sizeof(t_block));
	return ;
}

static int		ft_is_in_large_zone(t_block *b)
{
	size_t s;

	if (!b)
		return (0);
	s = sizeof(char *) > SIZE_POINTEUR_32
	? MIN_SIZE_LARGE_64 : MIN_SIZE_LARGE_32;
	if (b->size >= s)
	{
		return (1);
	}
	return (0);
}

static void		ft_fusion_block(t_block *b)
{
	if (b->next && b->is_free)
	{
		b->size += sizeof(t_block) + b->next->size;
		b->next = b->next->next;
		if (b->next)
		{
			b->next->prev = b;
		}
	}
	return ;
}

/*
** Free a block, set the block->is_free to 1 and fusion, if possible, the next
** and the prev block
*/

static void		ft_free_block(t_block *b)
{
	ft_fusion_block(b);
	b->is_free = 1;
	if (b->prev && b->prev->is_free)
	{
		ft_fusion_block(b->prev);
	}
	return ;
}


/*
** Free the pointeur
*/

void			free(void *ptr)
{
	t_block *b;
	void *zone;

	if (!base)
		return ;
	if (!ptr)
		return ;
	zone = ft_find_zone_block(ptr);
	b = ft_find_block(zone, ptr);
	if (!b)
		return ;
	if (ft_is_in_large_zone(b))
	{
		ft_free_large(b);
		return ;
	}
	if (b && b->data == ptr && !b->is_free)
		ft_free_block(b);
	return ;
}
