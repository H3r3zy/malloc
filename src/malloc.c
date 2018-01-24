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

t_memory *get_memory_block_by_data_ptr(void *data_ptr)
{
	t_memory *tmp = g_list;

	if (!tmp)
		return NULL;
	while (tmp->next && tmp->data != data_ptr) {
		tmp = tmp->next;
	}
	if (tmp->data == data_ptr)
		return tmp;
	return NULL;
}

void free(void *ptr)
{
	return;
	t_memory *block = get_memory_block_by_data_ptr(ptr);

	if (!block->next) {
		if (block->previous)
			block->previous->next = NULL;
		if (!block->previous)
			g_list = NULL;
		brk(sbrk(0) - block->size - sizeof(t_memory));
	}
	else {
		// Fusion des blocks
	}
	return;
}

void *malloc(size_t size)
{
	t_memory *block;
	t_memory *tmp = g_list;

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
	if (size == 0 && ptr) {
		free(ptr);
		return ptr;
	}
	if (!ptr)
		return malloc(size);

	t_memory *block = get_memory_block_by_data_ptr(ptr);
	if (!block)
		return malloc(size);
	t_memory *new_block = malloc(size);
	if (block->size <= size)
		memcpy(new_block->data, block->data, block->size);
	else
		memcpy(new_block->data, block->data, size);
	return new_block->data;
}