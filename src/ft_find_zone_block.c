/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_zone_block.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 16:36:24 by alex              #+#    #+#             */
/*   Updated: 2017/10/13 10:37:55 by aollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_block	*ft_find_zone_block(void *ptr)
{
	t_zone *zone;
	size_t s;

	zone = g_base;
	if (!zone)
		return (NULL);
	s = sizeof(char *) > SIZE_POINTEUR_32 ?
	SIZE_TINY_ZONE_64 : SIZE_TINY_ZONE_32;
	if ((unsigned char *)ptr >= (unsigned char *)zone->tiny->data
	&& (unsigned char *)ptr <= (unsigned char *)zone->tiny->data + s)
		return (zone->tiny);
	s = sizeof(char *) > SIZE_POINTEUR_32
	? SIZE_SMALL_ZONE_64 : SIZE_SMALL_ZONE_32;
	if ((unsigned char *)ptr >= (unsigned char *)zone->small->data
	&& (unsigned char *)ptr <= (unsigned char *)zone->small->data + s)
		return (zone->small);
	return (zone->large);
}
