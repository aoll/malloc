/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 15:20:33 by alex              #+#    #+#             */
/*   Updated: 2017/07/21 10:15:55 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/inc/libft.h"

#include <stdio.h>
#include <stdlib.h>

/*
** INCLUDE
*/

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

/*
** DEFINE
*/

# define KB 1024
# define MB 1048576  //1024 * 1024 == 1024 * KB

# define MIN_SIZE_SMALL_32 497
# define MIN_SIZE_SMALL_64 993

# define MIN_SIZE_LARGE_32 7681
# define MIN_SIZE_LARGE_64 15361

# define SIZE_TINY_ZONE_32 1 * MB
# define SIZE_TINY_ZONE_64 2 * MB

# define SIZE_SMALL_ZONE_32 8 * MB
# define SIZE_SMALL_ZONE_64 16 * MB

/*
** STRUCT
*/

typedef struct s_block t_block;

struct s_block
{
	size_t		size;
	t_block		*next;
	t_block		*prev;
	int			is_free;
	char		data[1];
};

typedef struct s_zone t_zone;

struct s_zone
{
	t_block *tiny;
	t_block *small;
	t_block *large;
};
/*
** GLOBAL
*/

void *base = NULL;

/*
** PROTOTYPES
*/

void	*ft_malloc(size_t size);

/*
**  Return the nearest integer dividing, bigger or equal,
**  by 8 or 4 (32 or 64 bits systeme)
*/

size_t	ft_align(size_t target)
{
	size_t new_size;
	size_t ref_align;

	ref_align = sizeof(char *);
	if (!(target % ref_align))
	{
		return (target);
	}
	new_size = target / ref_align * ref_align + ref_align;
	return (new_size);
}

/*
** Fusion the block and the next if the next is free
*/

void	ft_fusion_block(t_block *b)
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
	return;
}

/*
** Free a block, set the block->is_free to 1 and fusion, if possible, the next
** and the prev block
*/

void	ft_free_block(t_block *b)
{
	ft_fusion_block(b);
	b->is_free = 1;
	if (b->prev && b->prev->is_free)
	{
		ft_fusion_block(b->prev);
	}
	return;
}

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

t_block	*ft_find_zone_block(void *ptr)
{
	t_zone *zone;
	size_t s;

	zone = base;
	if (!zone)
	{
		return (NULL);
	}
	s = sizeof(char *) > 4 ? SIZE_TINY_ZONE_64 : SIZE_TINY_ZONE_32;
	if ((unsigned char *)ptr >= (unsigned char *)zone->tiny->data && (unsigned char *)ptr <= (unsigned char *)zone->tiny->data + s)
	{
		return (zone->tiny);
	}
	s = sizeof(char *) > 4 ? SIZE_SMALL_ZONE_64 : SIZE_SMALL_ZONE_32;
	if ((unsigned char *)ptr >= (unsigned char *)zone->small->data && (unsigned char *)ptr <= (unsigned char *)zone->small->data + s)
	{
		return (zone->small);
	}
	return (zone->large);
}


int	ft_is_in_large_zone(t_block *b)
{
	size_t s;

	if (!b)
		return (0);
	s = sizeof(char *) > 4 ? MIN_SIZE_LARGE_64 : MIN_SIZE_LARGE_32;
	if (b->size >= s)
	{
		return (1);
	}
	return (0);
}

/*
** Free a large zone in memory
*/

void	ft_free_large(t_block *b)
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
	printf("%s\n", "free for a large block");
	return;
}

/*
** Free the pointeur
*/

void	ft_free(void *ptr)
{
	t_block *b;
	void *zone;

	//TODO ?? check if correct ptr and from where is wiche zone
	zone = ft_find_zone_block(ptr);
	b = ft_find_block(zone, ptr);
	if (!b)
	{
		return ;
	}
	if (ft_is_in_large_zone(b))
	{
		ft_free_large(b);
		return ;
	}
	if (b && b->data == ptr && !b->is_free)
	{
		ft_free_block(b);
	}
	return;
}

/*
** Find a free block with a size equal or bigger than the parameter size
** else return null
*/

t_block	*ft_find_free_block(t_block *first, size_t size)
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
** Split the block in parameter and add new meta-data for the new block
*/

void	ft_split_block(t_block *b, size_t s)
{
	t_block	*new;

	new = (t_block *)(b->data + s);
	new->size = b->size - s - sizeof(t_block);
	new->next = b->next;
	new->is_free = 1;
	b->size = s;
	b->next = new;
	return;
}

t_block	*ft_create_zone(void *addr, size_t s)
{
	t_block *zone;

	zone = mmap(
		addr, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (!zone)
		return (NULL);
	zone->size = s - sizeof(t_block);
	zone->is_free = 1;
	zone->next = NULL;
	zone->prev = NULL;
	return (zone);
}

/*
**
*/

void	ft_init_zone_32()
{
	t_zone	*zone;

	zone = mmap(
		0, sizeof(t_zone), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (!zone)
	{
		return ;
	}
	zone->tiny = ft_create_zone(NULL, SIZE_TINY_ZONE_32);
	if (!zone->tiny)
	{
		return ;
	}
	zone->small = ft_create_zone(zone->tiny->data + zone->tiny->size, SIZE_SMALL_ZONE_32);
	zone->large = NULL;
	base = zone;
	return ;
}

void	ft_init_zone_64()
{
	t_zone	*zone;
	void *ptr;

	zone = mmap(
		0, sizeof(t_zone), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (!zone)
	{
		return ;
	}
	zone->tiny = ft_create_zone((unsigned char *)zone + sizeof(t_zone), SIZE_TINY_ZONE_64);
	if (!zone->tiny)
	{
		return ;
	}
	zone->small = ft_create_zone(NULL, SIZE_SMALL_ZONE_64);
	printf("%s\n", "hello swag");
	// ptr = zone->tiny->data + zone->tiny->size;
	zone->large = NULL;
	base = zone;
	return ;
}

/*
** Init memory zone
*/

void	*ft_init_zone(size_t s)
{
	t_block	*b;

	if (sizeof(char *) > 4)
	{
		ft_init_zone_64();
	}
	else
	{
		ft_init_zone_32();
	}
	if (!base)
		return (NULL);
	return (ft_malloc(s));
}

t_block	*ft_find_zone64(size_t s)
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

t_block	*ft_find_zone32(size_t s)
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

t_block	*ft_find_zone(size_t s)
{
	if (sizeof(char *) > 4)
	{
		return (ft_find_zone64(s));
	}
	return (ft_find_zone32(s));
}


/*
** Alloue memory space for a large zone
*/

void	*ft_malloc_large(size_t s)
{
	t_zone *zone;
	t_block *large;

	if (!base)
		return (NULL);
	zone = base;
	large = ft_create_zone(NULL, s + sizeof(t_block));
	if (!large)
	{
		return (NULL);
	}
	large->is_free = 0;
	if (zone->large)
	{
		zone->large->next = large;
		large->prev = zone->large;
	}
	else
	{
		zone->large = large;
	}
	return (large->data);
}

/*
** A fit malloc with only small zone, dev time
*/

void	*ft_malloc(size_t size)
{
	t_block	*b;
	size_t	s;
	t_block *zone;

	if (base)
	{
		s = ft_align(size);
		if ( (s >= MIN_SIZE_LARGE_64 && sizeof(char *) > 4 ) || (s >= MIN_SIZE_LARGE_32 && sizeof(char *) <= 4 ))
			return (ft_malloc_large(s));
		zone = ft_find_zone(size);
		b = ft_find_free_block(zone, s);
		if (b)
		{
			b->is_free = 0;
			if ((b->size - s) >= (sizeof(t_block) + sizeof(char *)))
				ft_split_block(b, size);
			return (b->data);
		}
		else
		{
			return (NULL);
		}
	}
	return (ft_init_zone(size));
}

/*
** Realoc function copy the ptr give in paramameter, in a new block
*/

void	*ft_realloc(void *ptr, size_t s)
{
	t_block *src;
	t_block *dest;
	void	*zone;
	int		i;

	if (!ptr)
		return (ft_malloc(s));
	if (!s)
	{
		ft_free(ptr);
		return (NULL);
	}
	zone = ft_find_zone_block(ptr);
	src = ft_find_block(zone, ptr);
	if (!src)
		return (ptr);
	dest = ft_malloc(s);
	while (i < s && i < src->size)
	{
		dest->data[i] = src->data[i];
		i++;
	}
	return (dest->data);
}

/*
** Print a pointeur adresse with hex'format
*/

void	ft_print_adress(long double adr)
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

void	ft_print_zone(t_block *b, char *name_zone)
{
	ft_putstr(name_zone); ft_putstr(" ( "); ft_putnbr((size_t)b); ft_putstr(" ) ");
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
		}
		b = b->next;
	}
	return ;
}

/*
** Display the malloc' s allocations by croisante adresse
*/

void	show_alloc_mem(void)
{
	t_zone	*zone;

	zone = base;
	if (!zone)
		return ;
	if (zone->tiny)
		ft_print_zone(zone->tiny, "TINY : ");
	if (zone->small)
		ft_print_zone(zone->small, "SMALL : ");
	if (zone->large)
		ft_print_zone(zone->large, "LARGE : ");
	return ;
}

#include <string.h>

int		main(void)
{
	int i;
	char *s1;
	char *s2;
	char *s3;
	char *s4;
	size_t m;

	// i = 0;
	// while (i < 1024)
	// {
	// 	s1 = ft_malloc(1024);
	// 	s1[0] = 42;
	// 	ft_free(s1);
	// 	i++;
	// }


	// s1 = ft_malloc(12);
	// s1[0]  = 42;
	// s2 = ft_malloc(1200);
	// s2[0]  = 42;
	// s3 = ft_malloc(100000);
	// s3[0]  = 42;
	s4 = ft_malloc(200000);
	s4[0]  = 42;
	// printf("%s\n", s3 );


	show_alloc_mem();

	// ft_free(s4);
	return (0);
}
