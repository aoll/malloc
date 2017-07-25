/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:07:51 by alex              #+#    #+#             */
/*   Updated: 2017/07/25 16:49:24 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static long long int	ft_find_adresse(void)
{
	t_zone	*zone;
	t_block	*tmp;

	if (!base)
		return (0);
	zone = base;
	if (zone->large)
	{
		tmp = zone->large;
		while (tmp && tmp->next)
			tmp = tmp->next;
		return ((
			ft_align((long long int)tmp->data + tmp->size, getpagesize())));
	}
	if (zone->small)
	{
		tmp = zone->small;
		while (tmp && tmp->next)
			tmp = tmp->next;
		return ((
			ft_align((long long int)tmp->data + tmp->size, getpagesize())));
	}
	return (0);
}

/*
** Alloue memory space for a large zone
*/

static void				*malloc_large(size_t s)
{
	t_zone *zone;
	t_block *large;
	t_block *tmp;

	if (!base)
		return (NULL);
	zone = base;
	large = ft_create_zone((void *)ft_find_adresse(),
	ft_align(s + sizeof(t_block), getpagesize()));
	if (!large)
		return (NULL);
	large->is_free = 0;
	if (zone->large)
	{
		tmp = zone->large;
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = large;
		large->prev = tmp;
	}
	if (!zone->large)
		zone->large = large;
	return (large->data);
}

/*
** Split the block in parameter and add new meta-data for the new block
*/

static void				ft_split_block(t_block *b, size_t s)
{
	t_block	*new;

	new = (t_block *)(b->data + s);
	new->size = b->size - s - sizeof(t_block);
	new->next = b->next;
	new->is_free = 1;
	b->size = s;
	b->next = new;
	return ;
}

/*
** Find a free block with a size equal or bigger than the parameter size
** else return null
*/

static t_block			*ft_find_free_block(t_block *first, size_t size)
{
	t_block *b;

	b = first;
	while (b && !(b->is_free && b->size >= size))
	{
		b = b->next;
	}
	return (b);
}

/*
** A fit malloc with only small zone, dev time
*/

void					*malloc(size_t size)
{
	t_block	*b;
	size_t	s;

	if (base)
	{
		s = ft_align(size, sizeof(char *));
		if ((s >= MIN_SIZE_LARGE_64 && sizeof(char *) > SIZE_POINTEUR_32)
		|| (s >= MIN_SIZE_LARGE_32 && sizeof(char *) <= SIZE_POINTEUR_32))
			return (malloc_large(s));
		b = ft_find_free_block(ft_find_zone(size), s);
		if (b)
		{
			b->is_free = 0;
			if ((b->size - s) >= (sizeof(t_block) + sizeof(char *)))
				ft_split_block(b, size);
			return (b->data);
		}
		if (!b)
			return (NULL);
	}
	return (ft_init_zone(size));
}
