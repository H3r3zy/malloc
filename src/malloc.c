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
//pthread_mutex_t thread_safe = PTHREAD_MUTEX_INITIALIZER;

t_block *fusion_block(t_block *block)
{
//	my_putstr("Fusion Block\n");
	if (block && block->next && block->next->free == FREE) {

		block->size += block->next->size;
		block->next = block->next->next;
		if (block->next)
			block->next->previous = block;
	}
	return (block);
}

t_block *split_block(t_block *block, size_t size)
{
//	my_putstr("Split Block\n");
	t_block *new_block = NULL;
	/*size_t tmp = getpagesize() - sizeof(t_block);
	while (tmp < size) {
		tmp += getpagesize();
	}
	//size = size + getpagesize() + sizeof(t_block);
	size = tmp;*/

	if (!block || block->size < size + sizeof(t_block) + 1)
		return (block);

	new_block = (t_block *)((char  *)(block + size + 1 + sizeof(t_block)));
	if (new_block >= sbrk(0))
		return (block);
	new_block->previous = block;
	new_block->next = block->next;
	if (new_block->next)
		new_block->next->previous = new_block;
	new_block->size = block->size - size - sizeof(t_block) - 1;
	new_block->free = FREE;
	block->size = size;
	block->next = new_block;
	return (block);
}
/*
t_block *get_block_by_data(void *ptr)
{
	t_block *tmp = g_list;

	while (tmp && tmp < sbrk(0) && (char *)(tmp + 1) != ptr)
		tmp = tmp->next;
	return (tmp);
}*/

void lock(void)
{
	pthread_mutex_lock(&thread_safe);
}

void unlock(void)
{
	pthread_mutex_unlock(&thread_safe);
}

t_block *get_block_by_data(void *ptr)
{
	t_block *block = (t_block *)ptr - 1;
	if (!g_list || block < g_list || block > sbrk(0))
		return (NULL);
	return (block);
}

void free(void *ptr)
{
	t_block *block = NULL;

	lock();
	block = get_block_by_data(ptr);
	if (block) {
		block->free = FREE;
		if (block->next && block->next->free == FREE)
			block = fusion_block(block);
		if (block->previous && block->previous->free == FREE)
			fusion_block(block->previous);
		/*if (g_list->free == FREE) {
			brk(g_list);
			g_list = NULL;
		}*/
	}
	unlock();
}

t_block *get_last_block(void)
{
	t_block *block = g_list;

	while (block && block->next) {
		block = block->next;
	}
	return (block);
}

size_t align(size_t size) {
	return size;
//	return ALIGN_8(size);
	//return MALLOC_ALIGN(size, sizeof(int *));
}

t_block *add_full_block(int min_size)
{
	t_block *block;
	t_block *tmp = get_last_block();
	int size = getpagesize();

	while (size <= min_size + sizeof(t_block))
		size += getpagesize();
	size += getpagesize() * MIN_PAGESIZE_NUMBER;

	block = sbrk(0);

	if (-1 == brk(block + size))
		return (NULL);
	block->next = NULL;
	block->free = FREE;
	block->size = size - sizeof(t_block);
	block->previous = tmp;
	if (tmp) {
		tmp->next = block;
	} else {
		g_list = block;
	}
	return (block);
}

void *malloc(size_t size)
{
	t_block *block = NULL;
	t_block *tmp = NULL;

	lock();
	size = align(size);
	tmp = g_list;
	while (tmp) {
		if (tmp->free == FREE && tmp->size >= size) {
			tmp = split_block(tmp, size);
			tmp->free = NOT_FREE;
			unlock();
			return ((char *)(tmp + 1));
		}
		tmp = tmp->next;
	}

	block = add_full_block(size);
	if (!block) {
		unlock();
		return (NULL);
	}
	block = split_block(block, size);
	block->free = NOT_FREE;
	unlock();
	return ((char *)(block + 1));
}

void move_block_data(t_block *dest, t_block *src)
{
	int size = dest->size > src->size ? src->size : dest->size;

	memmove((char *)(dest + 1), (char *)(src + 1), size);
}

void *realloc(void *ptr, size_t size)
{
	t_block *block = NULL;
	t_block *new_block = NULL;

	lock();
	if (size == 0 && ptr) {
		unlock();
		free(ptr);
		return (ptr);
	}
	size = align(size);
	if (!ptr) {
		unlock();
		return (malloc(size));
	}

	block = get_block_by_data(ptr);
	if (!block || size < block->size) {
		split_block(block, size);
		unlock();
		return (ptr);
	}

	if (block->next) {
		if (block->next->free == FREE && block->size + block->next->size + sizeof(t_block) >= size) {
			void *data = (char *)(fusion_block(block) + 1);
			unlock();
			return (data);
		}
		unlock();
		new_block = malloc(size);
		if (!new_block) {
			return (NULL);
		}
		lock();
		new_block = get_block_by_data(new_block);
		move_block_data(new_block, block);
		if (block != new_block) {
			unlock();
			free((char *)(block + 1));
		}
		new_block->free = NOT_FREE;
		unlock();
		return ((char *)(new_block + 1));
	}

	if (-1 == brk((char *)(block + size + sizeof(char *)))) {
		unlock();
		return (NULL);
	}
	block->size = size;
	block->free = NOT_FREE;
	unlock();
	return ((char *)(block + 1));
}

void *calloc(size_t num, size_t size)
{
	void *ptr = malloc(num * size);

	if (!ptr)
		return (NULL);
	lock();
	memset(ptr, 0, num * size);
	unlock();
	return (ptr);
}

void *tcmalloc(size_t num)
{
	return (malloc(num));
}