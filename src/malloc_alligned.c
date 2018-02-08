/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include "malloc.h"

pthread_mutex_t secure_thread = PTHREAD_MUTEX_INITIALIZER;
unsigned long int allocated = 0;
t_block *g_list = NULL;
const int 	NOT_FREE = 0;
const int 	FREE = 1;
const int 	MIN_PAGESIZE_NUMBER = 32;

void *malloc(size_t size)
{
	t_block *block = NULL;
	t_block *previous = NULL;

	size = DATA_SIZE(ALIGN_UP(BLOCK_SIZE(size), ALIGNEMENT));
	block = g_list;
	LOCK(&secure_thread);
	while (block) {
		if (block->free == FREE && (size_t)block->size >= size) {
			block = split_block(block, size);
			block->free = NOT_FREE;
			UNLOCK(&secure_thread);
			return (HEADER2DATA(block));
		}
		previous = block;
		block = block->next;
	}
	block = morecore(previous, size);
	block = split_block(block, size);
	if (block)
		block->free = NOT_FREE;
	UNLOCK(&secure_thread);
	return (VALIDBLOCK(HEADER2DATA(block)));
}