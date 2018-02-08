/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "malloc.h"

void show_alloc_mem(void)
{
	t_block *tmp = g_list;

	printf("break : 0x%lX\n", (unsigned long int)sbrk(0));
	while (tmp) {
		printf("0x%lX - 0x%lX : %i bytes",
			(unsigned long int)HEADER2DATA(tmp),
			(unsigned long int)(HEADER2DATA(tmp) + tmp->size),
			tmp->size);
		printf("\n");
		tmp = tmp->next;
	}
}