/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_align.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:00:48 by alex              #+#    #+#             */
/*   Updated: 2017/07/25 15:42:49 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**  Return the nearest integer dividing, bigger or equal,
**  by 8 or 4 (32 or 64 bits systeme)
*/

long long int	ft_align(long long int target, long long int ref_align)
{
	long long int new_size;

	if (!(target % ref_align))
	{
		return (target);
	}
	new_size = target / ref_align * ref_align + ref_align;
	return (new_size);
}
