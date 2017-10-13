/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 15:07:28 by alex              #+#    #+#             */
/*   Updated: 2017/10/13 10:34:46 by aollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

/*
** INCLUDE
*/

# include "libft.h"
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/mman.h>

/*
** DEFINE
*/

# define SIZE_POINTEUR_32 4

# define KB 1024
# define MB 1048576

# define MIN_SIZE_SMALL_32 497
# define MIN_SIZE_SMALL_64 993

# define MIN_SIZE_LARGE_32 7681
# define MIN_SIZE_LARGE_64 15361

# define SIZE_TINY_ZONE_32 1 * MB
# define SIZE_TINY_ZONE_64 2097152

# define SIZE_SMALL_ZONE_32 8 * MB
# define SIZE_SMALL_ZONE_64 16777216

/*
** STRUCT
*/

typedef struct s_block	t_block;

struct			s_block
{
	size_t		size;
	t_block		*next;
	t_block		*prev;
	int			is_free;
	char		data[1];
};

typedef struct s_zone	t_zone;

struct			s_zone
{
	t_block		*tiny;
	t_block		*small;
	t_block		*large;
};

/*
** GLOBAL
*/

void *g_base;

/*
** PROTOTYPES
*/

void			ft_free(void *ptr);
long long int	ft_align(long long int target, long long int ref_align);
void			show_alloc_mem(void);
void			*ft_realloc(void *ptr, size_t s);
void			*ft_malloc(size_t size);
void			*ft_init_zone(size_t s);
t_block			*ft_find_zone(size_t s);
t_block			*ft_find_zone_block(void *ptr);
t_block			*ft_create_zone(void *addr, size_t s);
t_block			*ft_find_block(void *src, void *ptr);

#endif
