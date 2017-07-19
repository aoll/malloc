/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 15:20:33 by alex              #+#    #+#             */
/*   Updated: 2017/07/19 14:39:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

/*
** INCLUDE
*/

#include <stdio.h>
#include <sys/mman.h>

/*
** DEFINE
*/

# define KB 1024
# define MB 1048576  //1024 * 1024 == 1024 * KB
/* small_zone == 16 * MB */
# define min_size_small 993
# define min_size_large 15360

# define small_zone 16 * MB

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

/*
** Free the pointeur
*/

void	ft_free(void *ptr)
{
	t_block *b;
	void *zone;

	//TODO check if correct ptr and from where is wiche zone
	zone = base;
	b = ft_find_block(zone, ptr);
	if (b && b->data == ptr)
	{
		printf("%s\n", "found");
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

t_block	*ft_create_zone(size_t s)
{
	t_block *zone;

	zone = mmap(
		0, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (!zone)
		return (NULL);
	zone->size = s - sizeof(t_block);
	zone->is_free = 1;
	zone->next = NULL;
	return (zone);
}


/*
** Init a memory zone
*/

void	*ft_init_zone(size_t s)
{
	t_block	*b;

	b = ft_create_zone(16 * MB);
	if (!b)
	{
		return (NULL);
	}
	base = b;
	return (ft_malloc(s));
}

/*
** A fit malloc with only small zone, dev time
*/

void	*ft_malloc(size_t size)
{
	t_block	*b;
	size_t	s;

	if (base)
	{
		s = ft_align(size);
		b = ft_find_free_block(base, s);
		if (b)
		{
			b->is_free = 0;
			if ((b->size - s) >= (sizeof(t_block) + sizeof(char *)))
			{
				ft_split_block(b, size);
			}
			return (b->data);
		}
		else
		{
			return (NULL);
		}
	}
	else
	{
		return (ft_init_zone(size));
	}
	return (NULL);
}

/*
** Realoc function copy the ptr give in paramameter, in a new block
*/

void	*ft_realloc(void *ptr, size_t s)
{
	t_block *src;
	t_block *dest;
	void	*zone;

	zone = base;
	src = ft_find_block(zone, ptr);
	return;
}

/*
** si tu cherches une meuf chaude : 0637708000
*/

int		main(void)
{
	int i;
	char *s;

	i = 0;
	while (i < 1024)
	{
		s = ft_malloc(1024);
		s[0] = 42;
		// s++;
		ft_free(s);
		i++;
	}
	return (0);
}
