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

char is_in_range(void *ptr, t_memory *block) {
	return ((void *) block->data <= ptr && (void *) block->data + block->size >= ptr);
}

t_memory *get_memory_block_by_data_ptr(void *data_ptr)
{
	t_memory *tmp = g_list;

	if (!tmp)
		return NULL;
	while (tmp->next && !is_in_range(data_ptr, tmp)) {
		tmp = tmp->next;
	}
	if (is_in_range(data_ptr, tmp)) {
//		printf("MATCH %p :: %p :: %p\n", data_ptr, tmp->data, tmp->data + tmp->size);
		return tmp;
	}
	return NULL;
}

void free(void *ptr)
{
	t_memory *block = get_memory_block_by_data_ptr(ptr);
	t_memory *previous = NULL;

	if (!block)
		return;
	block->free = FREE;
	previous = block->previous;
	if (!block->next) {
		brk(block);
		if (previous) {
			previous->next = NULL;
			if (previous->free == FREE)
				free(previous->data);
		}
		if (!previous)
			g_list = NULL;
	}
}

void *malloc(size_t size)
{
	//write(1, "Malloc\n", 7);
	t_memory *block;
	t_memory *tmp = g_list;

	/**
	 * Si block deja dispo
	 */
	while (tmp && tmp->next) {
		if (tmp->free == FREE && tmp->size >= size) {
			tmp->free = NOT_FREE;
			return tmp->data;
		}
		tmp = tmp->next;
	}
	tmp = g_list;

	block = sbrk(0);
	if (-1 == brk(block + size + sizeof(t_memory))) {
		write(1, "Erreur1\n", 8);
		return NULL;
	}
	block->next = NULL;
	block->free = NOT_FREE;
	block->ptr = block;
	block->size = size;
	while (tmp && tmp->next)
		tmp = tmp->next;
	block->previous = (tmp) ? (tmp) : (NULL);
	if (tmp)
		tmp->next = block;
	else
		g_list = block;
//	write(1, "FINloc\n", 7);
	return block->data;
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

void *realloc(void *ptr, size_t size)
{
	t_memory *new_block;
	//write(1, "REALLOC\n", 8);
	if (size == 0 && ptr) {
		free(ptr);
		return ptr;
	}
	if (!ptr)
		return malloc(size);

	t_memory *block = get_memory_block_by_data_ptr(ptr);
	if (!block)
		return malloc(size);
	if (block->next) {
		new_block = malloc(size);
	}
	else {
		new_block = block;
		if (-1 == brk(block + size + sizeof(t_memory))) {
			return NULL;
		}
		new_block->size = size;
		return new_block->data;
	}
	if (!new_block)
		return NULL;

	if (block->size <= size)
		memcpy(new_block->data, block->data, block->size);
	else
		memcpy(new_block->data, block->data, size);
	free(block->data);
	return new_block->data;
}