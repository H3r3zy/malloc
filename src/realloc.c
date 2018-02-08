/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "malloc.h"

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
	if (size < (size_t)block->size) {
		LOCK(&secure_thread);
		split_block(block, size);
		UNLOCK(&secure_thread);
		return (ptr);
	}
	if (block->next) {
		if (block->next->free == FREE &&
			block->size + BLOCK_SIZE(block->next->size) >= size) {
			LOCK(&secure_thread);
			data = VALIDBLOCK(HEADER2DATA(fusion_block(block)));
			UNLOCK(&secure_thread);
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
	LOCK(&secure_thread);
	morecore(block, DATA_SIZE(size));
	block = fusion_block(block);
	block = split_block(block, DATA_SIZE(size));
	UNLOCK(&secure_thread);
	block->free = NOT_FREE;
	return (VALIDBLOCK(HEADER2DATA(block)));
}
