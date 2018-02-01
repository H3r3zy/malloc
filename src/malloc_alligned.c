/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <stddef.h>
#include <memory.h>
#include <unistd.h>
#include <pthread.h>
#include "malloc.h"

t_block *g_list = NULL;

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
		new_block = (t_block *)((char *)(block + size));
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

t_block *get_last_block(void)
{
	t_block *block = g_list;

	while (block && block->next) {
		block = block->next;
	}
	return (block);
}

t_block *morecore(int size)
{
	t_block *block;
	t_block *tmp = get_last_block();
	int new_size = PAGESIZE;

	while (new_size < BLOCK_SIZE(size))
		new_size += PAGESIZE;
	new_size += PAGESIZE * MIN_PAGESIZE_NUMBER;
	block = sbrk(0);
	if (-1 == brk((char *)(block + new_size)))
		return (NULL);
	block->next = NULL;
	block->free = FREE;
	block->size = DATA_SIZE(new_size);
	block->previous = tmp;
	if (tmp) {
		tmp->next = block;
	} else {
		g_list = block;
	}
	return (block);
}

void free(void *ptr)
{
	t_block *block = NULL;

	LOCK(&thread_safe);
	block = DATA2HEADER(ptr);
	if (block && g_list && VALIDBLOCK(block)) {
		block->free = FREE;
		if (block->next && block->next->free == FREE)
			block = fusion_block(block);
		if (block->previous && block->previous->free == FREE)
			fusion_block(block->previous);
	}
	UNLOCK(&thread_safe);
}

void *malloc(size_t size)
{
	t_block *block = NULL;

	LOCK(&thread_safe);
	size = DATA_SIZE(ALIGN_UP(BLOCK_SIZE(size), ALIGNEMENT));
	block = g_list;
	while (block) {
		if (block->free == FREE && block->size >= size) {
			block = split_block(block, size);
			block->free = NOT_FREE;
			UNLOCK(&thread_safe);
			return (HEADER2DATA(block));
		}
		block = block->next;
	}
	block = morecore(size);
	block = split_block(block, size);
	if (block)
		block->free = NOT_FREE;
	UNLOCK(&thread_safe);
	return (VALIDBLOCK(HEADER2DATA(block)));
}

void *realloc(void *ptr, size_t size)
{
	t_block *block = VALIDBLOCK(DATA2HEADER(ptr));
	t_block *new_block = NULL;

	LOCK(&thread_safe);
	if (size == 0 && ptr) {
		UNLOCK(&thread_safe);
		free(ptr);
		return (ptr);
	}
	size = DATA_SIZE(ALIGN_UP(BLOCK_SIZE(size), ALIGNEMENT));
	if (!ptr || !g_list || !block) {
		UNLOCK(&thread_safe);
		return (malloc(size));
	}
	if (size < block->size) {
		split_block(block, size);
		UNLOCK(&thread_safe);
		return (ptr);
	}
	if (block->next) {
		if (block->next->free == FREE && block->size + BLOCK_SIZE(block->next->size) >= size) {
			void *data = VALIDBLOCK(HEADER2DATA(fusion_block(block)));
			UNLOCK(&thread_safe);
			return (data);
		}
		UNLOCK(&thread_safe);
		new_block = malloc(size);
		if (!new_block)
			return (NULL);
		LOCK(&thread_safe);
		new_block = DATA2HEADER(new_block);
		DATACPY(new_block, block);
		if (block != new_block) {
			UNLOCK(&thread_safe);
			free(VALIDBLOCK(HEADER2DATA(block)));
			LOCK(&thread_safe);
		}
		new_block->free = NOT_FREE;
		UNLOCK(&thread_safe);
		return (VALIDBLOCK(HEADER2DATA(new_block)));
	}
	if (-1 == brk(block + BLOCK_SIZE(size))) {
		UNLOCK(&thread_safe);
		return (NULL);
	}
	block->size = size;
	block->free = NOT_FREE;
	UNLOCK(&thread_safe);
	return (VALIDBLOCK(HEADER2DATA(block)));
}


void *calloc(size_t num, size_t size)
{
	void *ptr = malloc(num * size);

	if (ptr) {
		LOCK(&thread_safe);
		memset(ptr, 0, num * size);
		UNLOCK(&thread_safe);
	}
	return (ptr);
}