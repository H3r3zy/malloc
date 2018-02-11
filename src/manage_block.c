/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "malloc.h"

t_block *fusion_block(t_block *block)
{
	if (block && block->next && block->next->free == FREE) {
		block->size += BLOCK_SIZE(block->next->size);
		block->next = block->next->next;
		if (block->next)
			block->next->previous = block;
	}
	return (block);
}

t_block *split_block(t_block *block, int size)
{
	t_block *new_block = NULL;

	size = ALIGN_UP(BLOCK_SIZE(size), ALIGNEMENT);
	if (block && block->size >= size) {
		new_block = (t_block *)((char *)block + size);
		if (!VALIDBLOCK(new_block))
			return (block);
		new_block->previous = block;
		new_block->next = block->next;
		if (new_block->next)
			new_block->next->previous = new_block;
		new_block->size = block->size - size;
		new_block->free = FREE;
		block->size = DATA_SIZE(size);
		block->next = new_block;
	}
	return (block);
}

void move_block_data(t_block *dest, t_block *src)
{
	int size = dest->size > src->size ? src->size : dest->size;

	memmove((char *)(dest + 1), (char *)(src + 1), size);
}

t_block *morecore(t_block *end, int size)
{
	t_block *block;
	int new_size =
		ALIGN_UP(size, PAGESIZE) + PAGESIZE * MIN_PAGESIZE_NUMBER;

	allocated += new_size;
	block = sbrk(0);
	if (-1 == brk((char *)(block) + new_size))
		return (NULL);
	block->next = NULL;
	block->free = FREE;
	block->size = DATA_SIZE(new_size);
	block->previous = end;
	if (end) {
		end->next = block;
	} else {
		g_list = block;
	}
	return (block);
}
