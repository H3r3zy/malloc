/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef PSU_2017_MALLOC_MALLOC_H
#define PSU_2017_MALLOC_MALLOC_H

typedef struct s_memory {
	struct s_memory *previous;
	struct s_memory *next;
	int size;
	int free;
	char data[1];
} t_memory;

#define NOT_FREE 0
#define FREE 1

#define MIN_PAGESIZE 10

t_memory *get_memory_block_by_data_ptr(void *data_ptr);
int get_memory_len();
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

#endif //PSU_2017_MALLOC_MALLOC_H
