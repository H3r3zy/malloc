/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "malloc.h"

void free(void *ptr)
{
	t_block *block = NULL;

	LOCK(&secure_thread);
	block = DATA2HEADER(ptr);
	if (block && g_list && VALIDBLOCK(block)) {
		block->free = FREE;
		if (block->next && block->next->free == FREE)
			block = fusion_block(block);
		if (block->previous && block->previous->free == FREE)
			fusion_block(block->previous);
	}
	UNLOCK(&secure_thread);
}
