/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include "malloc.h"

t_memory *g_list = NULL;

void my_putstr(char *str) {
	write(1, str, strlen(str));
}

void my_putchar(char c)
{
	write(1, &c, 1);
}

int     my_put_nbr(int n)
{
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
	{
		my_put_nbr(n / 10);
		my_putchar(n % 10 + '0');
	}
	if (n < 10)
	{
		my_putchar(n % 10 + '0');
	}
	return (n);
}

int get_memory_len() {
	int i = 0;
	t_memory *tmp = g_list;

	while (tmp) {
/*		my_put_nbr(i);
		my_putstr(" :: ");
		my_put_nbr(tmp);
		my_putstr("\n");*/
		i++;
		tmp = tmp->next;
	}
	return i;
}

t_memory *split_block_of_size(t_memory *block, size_t size)
{
	t_memory *new_block = NULL;

	/*my_putstr("Split block of size :: ");
	my_put_nbr(block->size);
	my_putstr(" in block of size :: ");
	my_put_nbr(size);
	my_putstr(" and :: ");
	my_put_nbr(block->size - size - sizeof(t_memory));
	my_putstr("\n");*/
	if (block && size > 0 && size < block->size - sizeof(t_memory)) {
		//my_putstr("split block\n");
		new_block = block + size + sizeof(t_memory);
		new_block->previous = block;
		new_block->size = block->size - size - sizeof(t_memory);
		new_block->free = FREE;
		new_block->next = block->next;
		block->next = new_block;
		block->size = size;
	}
	return (block);
}

t_memory *fusion_block_with_next(t_memory *block)
{
	if (block->next && block->next->free == FREE) {
		block->size += sizeof(t_memory) + block->next->size - sizeof(char *);
		block->next = block->next->next;
	}
	return block;
}

t_memory *get_last_block()
{
	t_memory *tmp = g_list;

	while (tmp && tmp->next)
		tmp = tmp->next;
	return tmp;
}

char is_in_range(void *ptr, t_memory *block) {
	if (!block)
		return 0;
	return ((void *) block->data <= ptr && (void *) block->data + block->size >= ptr);
}

t_memory *get_memory_block_by_data_ptr(void *data_ptr)
{
	t_memory *tmp = g_list;

	if (!tmp || !data_ptr)
		return NULL;
	while (tmp && !is_in_range(data_ptr, tmp)) {
		tmp = tmp->next;
	}
	return tmp;
	/*if (is_in_range(data_ptr, tmp)) {
		return tmp;
	}*/
	//printf("blockTest :: %p || ", tmp);
	tmp =  data_ptr - sizeof(t_memory) + sizeof(char *);
	//printf("Data :: %p || block :: %p\n", data_ptr, tmp);
	return tmp;
	//my_putstr("\n");
	//return NULL;
}

void free(void *ptr)
{
	//return;
	t_memory *block = get_memory_block_by_data_ptr(ptr);
	//ét_memory *previous = NULL;

	if (!block || !ptr)
		return;
	/*my_putstr("free\n=> data :: ");
	my_put_nbr(ptr);
	my_putstr("\n=>block :: ");
	my_put_nbr(block);
	my_putstr("\n");*/
	block->free = FREE;
	if (block->next && block->next->free == FREE)
		block = fusion_block_with_next(block);
	if (block->previous && block->previous->free == FREE)
		fusion_block_with_next(block->previous);
	/*if (g_list->free == FREE) {
		brk(g_list);
		g_list = NULL;
	}*/
	/*previous = block->previous;
	if (!block->next) {
		brk(block);
		if (previous) {
			previous->next = NULL;
			//if (previous->free == FREE)
			//	free(previous->data);
		}
		if (!previous)
			g_list = NULL;
	}*/
}
/*
void *malloc(size_t size)
{
	t_memory *block;
	t_memory *tmp = g_list;

	while (tmp && tmp->next) {
		if (tmp->free == FREE && tmp->size >= size) {
			tmp->free = NOT_FREE;
			return tmp->data;
		}
		tmp = tmp->next;
	}
	tmp = get_last_block();

	block = sbrk(0);
	if (-1 == brk(block + size + sizeof(t_memory))) {
		//my_putstr("Error malloc\n");
		return NULL;
	}
	block->next = NULL;
	block->free = NOT_FREE;
	block->size = size;
	block->previous = (tmp) ? (tmp) : (NULL);
	block->data;
	if (tmp) {
		tmp->next = block;
	}
	else {
		g_list = block;
	}
	return block->data;
}*/

size_t get_needed_pagesize(size_t min)
{
	int pagesize = getpagesize();
	size_t needed = pagesize;

	while (needed < min + sizeof(t_memory)) {
		needed += pagesize;
	}
	needed += pagesize * MIN_PAGESIZE;
	return (needed);
}

void *malloc(size_t size)
{
	t_memory *block;
	t_memory *tmp = g_list;

	while (tmp) {
		/*my_putstr("Free ? :: ");
		my_put_nbr(tmp->free);
		my_putstr("\n");*/
		if (tmp->free == FREE && tmp->size >= size + sizeof(t_memory)) {
			//my_putstr("Uesh je split mon block\n");
			tmp = split_block_of_size(tmp, size);
			tmp->free = NOT_FREE;
			return tmp->data;
		}
		tmp = tmp->next;
	}
	tmp = get_last_block();
	// Add pagesized block
	block = sbrk(0);
	size_t sbrk_size = get_needed_pagesize(size);

	/*my_putstr("Adding Full Block of size :: ");
	my_put_nbr(sbrk_size);
	my_putstr("\n");*/

	if (-1 == brk(block + sbrk_size))
		return NULL;
	block->next = NULL;
	block->free = NOT_FREE;
	block->size = sbrk_size - sizeof(t_memory);
	block->previous = tmp;
	if (tmp) {
		tmp->next = block;
	} else {
	//	my_putstr("First block\n");
		g_list = block;
	}
	block = split_block_of_size(block, size);
	return block->data;
}
/*
void *realloc(void *ptr, size_t size)
{
	t_memory *new_block;
	if (size == 0 && ptr) {
		free(ptr);
		return ptr;
	}
	if (!ptr) {
		return malloc(size);
	}

	t_memory *block = ptr - sizeof(t_memory);
	if (!block) {
		return malloc(size);
	}
	if (block->next) {
		new_block = malloc(size) - sizeof(t_memory);
	}
	else {
		new_block = block;
		if (-1 == brk(block + size + sizeof(t_memory)))
			return NULL;
		new_block->size = size;
		return new_block->data;
	}

	if (block->size <= size) {
		memmove(new_block->data, block->data, block->size);
	} else {
		memmove(new_block->data, block->data, size);
	}
	free(block->data);
	return new_block->data;
}
*/

void *realloc(void *ptr, size_t size)
{
	t_memory *new_block;
	if (size == 0 && ptr) {
		free(ptr);
		return ptr;
	}
	if (!ptr) {
		return malloc(size);
	}

	t_memory *block = get_memory_block_by_data_ptr(ptr);
	if (!block || size < block->size)
		return ptr;
	if (block->next) {
		if (block->next->free == FREE && block->size + block->next->size + sizeof(t_memory) >= size) {
	//		my_putstr("Fusion maggle\n");
			return (fusion_block_with_next(block)->data);
		}
		new_block = malloc(size);
		if (!new_block)
			return NULL;

		//printf("%p :: %p\n", new_block, get_memory_block_by_data_ptr(new_block));
		new_block = get_memory_block_by_data_ptr(new_block);
		if (block->size <= size)
			memcpy(new_block->data, block->data, block->size);
		else
			memcpy(new_block->data, block->data, size);
	} else {
	//	my_putstr("Ici\n");
		new_block = block;
		if (-1 == brk(block->data + size + 1000))
		{
	//		my_putstr("Error maggle\n");
			return NULL;
		}
		new_block->size = size;
	//	my_putstr("Extend Bloc\n");
		return new_block->data;
	}

	if (block->size <= size) {
		memcpy(new_block->data, block->data, block->size);
	} else {
		memcpy(new_block->data, block->data, size);
	}
	if (block != new_block)
		free(block->data);
	//my_putstr("Putin\n");
	return (new_block->data);
}

void *calloc(size_t num, size_t size)
{
	return malloc(num * size);
}