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
	size_t size;
	char free;
	void *ptr;
	char data[1];
} t_memory;

#define NOT_FREE 0
#define FREE 1

#endif //PSU_2017_MALLOC_MALLOC_H
