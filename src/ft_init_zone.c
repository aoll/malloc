/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_zone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:20:58 by alex              #+#    #+#             */
/*   Updated: 2017/07/25 09:34:24 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		ft_init_zone_32(void)
{
	t_zone			*zone;
	long long int	adr;
	int				page_size;

	page_size = getpagesize();
	zone = mmap((void *)0, page_size, PROT_READ | PROT_WRITE,
	MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zone == (void *) - 1)
		return ;
	adr = (long long int)(zone + page_size);
	zone->tiny = ft_create_zone((void *)adr,
	ft_align(SIZE_TINY_ZONE_32,page_size));
	if (!zone->tiny)
		return ;
	adr = (long long int)(
		zone + page_size + ft_align(SIZE_TINY_ZONE_32, page_size));
	zone->small = ft_create_zone(
		(void *)adr, ft_align(SIZE_SMALL_ZONE_32,page_size));
	zone->large = NULL;
	base = zone;
	return ;
}

static void		ft_init_zone_64(void)
{
	t_zone			*zone;
	long long int	adr;
	int				page_size;

	page_size = getpagesize();
	zone = mmap((void *)0, page_size, PROT_READ | PROT_WRITE,
	MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zone == (void *) - 1)
		return ;
	adr = (long long int)(zone + page_size);
	zone->tiny = ft_create_zone((void *)adr,
	ft_align(SIZE_TINY_ZONE_64, page_size));
	if (!zone->tiny)
		return ;
	adr = (long long int)(
		zone + page_size + ft_align(SIZE_TINY_ZONE_64, page_size));
	zone->small = ft_create_zone((void *)adr,
	ft_align(SIZE_SMALL_ZONE_64, page_size));
	zone->large = NULL;
	base = zone;
	return ;
}


/*
** Init memory zone
*/

void		*ft_init_zone(size_t s)
{
	if (sizeof(char *) > SIZE_POINTEUR_32)
	{
		ft_init_zone_64();
	}
	else
	{
		ft_init_zone_32();
	}
	if (!base)
		return (NULL);
	return (malloc(s));
}
