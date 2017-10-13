/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/17 15:20:33 by alex              #+#    #+#             */
/*   Updated: 2017/07/24 17:07:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Fusion the block and the next if the next is free
*/

int		main(void)
{
	int i;
	char *s1;
	char *s2;
	char *s3;
	char *s4;
	size_t m;

	// getrlimit(RLIMIT_AS, &r);
	// printf("s %lld\n", (long long int)r.rlim_cur);
	// printf("h %lld\n", (long long int)r.rlim_max);
	// i = 0;
	// while (i < 1024)
	// {
	// 	s1 = malloc(1024);
	// 	s1[0] = 42;
	// 	free(s1);
	// 	i++;
	// }

	i = 0;
	while (i < 1024)
	{
		s1 = malloc(16000);
		s1[0]  = 42;
		i++;
	}
	// s2 = malloc(1200);
	// s2[0]  = 42;
	// s3 = malloc(100000);
	// s3[0]  = 42;
	// s4 = malloc(200000);
	// s4[0]  = 42;
	// printf("%s\n", s3 );


	show_alloc_mem();

	// free(s4);
	return (0);
}
