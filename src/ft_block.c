/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_block.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 16:40:37 by alex              #+#    #+#             */
/*   Updated: 2017/07/24 17:07:08 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Find a block with the adresse of the poiteur
*/

t_block	*ft_find_block(void *src, void *ptr)
{
	t_block *b;

	b = src;
	while (b && b->data != ptr)
	{
		b = b->next;
	}
	return (b);
}
