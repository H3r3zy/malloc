/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <stddef.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <pthread.h>
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
	if (-1 == brk((char *)(block + new_size)))
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
	t_block *previous = NULL;

	size = DATA_SIZE(ALIGN_UP(BLOCK_SIZE(size), ALIGNEMENT));
	block = g_list;
	LOCK(&thread_safe);
	while (block) {
		if (block->free == FREE && block->size >= size) {
			block = split_block(block, size);
			block->free = NOT_FREE;
			UNLOCK(&thread_safe);
			return (HEADER2DATA(block));
		}
		previous = block;
		block = block->next;
	}
	block = morecore(previous, size);
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
	void *data = NULL;

	if (size == 0 && ptr) {
		free(ptr);
		return (ptr);
	}
	size = DATA_SIZE(ALIGN_UP(BLOCK_SIZE(size), ALIGNEMENT));
	if (!ptr || !g_list || !block) {
		return (malloc(size));
	}
	if (size < block->size) {
		LOCK(&thread_safe);
		split_block(block, size);
		UNLOCK(&thread_safe);
		return (ptr);
	}
	if (block->next) {
		if (block->next->free == FREE &&
			block->size + BLOCK_SIZE(block->next->size) >= size) {
			LOCK(&thread_safe);
			data = VALIDBLOCK(HEADER2DATA(fusion_block(block)));
			UNLOCK(&thread_safe);
			return (data);
		}
		new_block = malloc(size);
		if (!new_block)
			return (NULL);
		new_block = DATA2HEADER(new_block);
		DATACPY(new_block, block);
		if (block != new_block) {
			free(VALIDBLOCK(HEADER2DATA(block)));
		}
		new_block->free = NOT_FREE;
		return (VALIDBLOCK(HEADER2DATA(new_block)));
	}
	LOCK(&thread_safe);
	morecore(block, DATA_SIZE(size));
	block = fusion_block(block);
	block = split_block(block, DATA_SIZE(size));
	UNLOCK(&thread_safe);
	//block->size = DATA_SIZE(size);
	block->free = NOT_FREE;
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