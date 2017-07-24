/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_zone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:50:59 by alex              #+#    #+#             */
/*   Updated: 2017/07/24 15:52:22 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_block	*ft_find_zone64(size_t s)
{
	t_zone *zone;

	if (!base)
	{
		return (NULL);
	}
	zone = base;
	if (s < MIN_SIZE_SMALL_64)
	{
		return (zone->tiny);
	}
	else if (s < MIN_SIZE_LARGE_64)
	{
		return (zone->small);
	}
	return (zone->large);
}

static t_block	*ft_find_zone32(size_t s)
{
	t_zone *zone;

	zone = base;
	if (!base)
	{
		return (NULL);
	}
	if (s < MIN_SIZE_SMALL_32)
	{
		return (zone->tiny);
	}
	else if (s < MIN_SIZE_LARGE_32)
	{
		return (zone->small);
	}
	return (zone->large);
}

t_block			*ft_find_zone(size_t s)
{
	if (sizeof(char *) > SIZE_POINTEUR_32)
	{
		return (ft_find_zone64(s));
	}
	return (ft_find_zone32(s));
}
