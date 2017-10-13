/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_zone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 16:39:22 by alex              #+#    #+#             */
/*   Updated: 2017/10/13 10:37:27 by aollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_block	*ft_create_zone(void *addr, size_t s)
{
	t_block			*zone;
	struct rlimit	r;

	getrlimit(RLIMIT_AS, &r);
	if ((long long int)r.rlim_max != -1 || (long long int)r.rlim_cur != -1)
		return (NULL);
	zone = mmap(
		addr, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zone == (void *)-1)
		return (NULL);
	zone->size = s - sizeof(t_block);
	zone->is_free = 1;
	zone->next = NULL;
	zone->prev = NULL;
	return (zone);
}
