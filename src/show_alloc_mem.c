/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:03:43 by alex              #+#    #+#             */
/*   Updated: 2017/07/24 15:05:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Print a pointeur adresse with hex'format
*/

static void				ft_print_adress(long double adr)
{
	long int	simple;
	long double	d;

	simple = adr / 16;
	d = adr / 16 - simple;
	if (simple > 0)
	{
		ft_print_adress(simple);
	}
	simple = d * 16;
	if (simple > 9)
	{
		simple = 65 + simple - 10;
	}
	else
	{
		simple += '0';
	}
	write(1, &simple, 1);
	return ;
}

static long long int	ft_print_zone(t_block *b, char *name_zone)
{
	long long int size_octets;

	size_octets = 0;
	ft_putstr(name_zone);
	ft_putstr("0x");
	ft_print_adress((size_t)b);
	ft_putstr("\n");
	while (b)
	{
		if (!b->is_free)
		{
			ft_putstr("0x");
			ft_print_adress((size_t)b->data);
			ft_putstr(" - ");
			ft_putstr("0x");
			ft_print_adress((size_t)(b->data + b->size));
			ft_putstr(" : ");
			ft_putnbr(b->size);
			ft_putstr(" octets\n");
			size_octets += b->size;
		}
		b = b->next;
	}
	return (size_octets);
}

/*
** Display the malloc' s allocations by croisante adresse
*/

void					show_alloc_mem(void)
{
	t_zone			*zone;
	long long int	total_size;

	zone = base;
	total_size = 0;
	if (!zone)
		return ;
	if (zone->tiny)
		total_size += ft_print_zone(zone->tiny, "TINY : ");
	if (zone->small)
		total_size += ft_print_zone(zone->small, "SMALL : ");
	if (zone->large)
		total_size += ft_print_zone(zone->large, "LARGE : ");
	ft_putstr("Total : ");
	ft_putnbr((int)total_size);
	ft_putstr(" octets\n");
	return ;
}
