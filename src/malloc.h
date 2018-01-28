/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef PSU_2017_MALLOC_MALLOC_H
#define PSU_2017_MALLOC_MALLOC_H

typedef struct s_block {
	struct s_block *previous;
	struct s_block *next;
	int size;
	int free;
//	char data[1];
} t_block;

#define NOT_FREE 0
#define FREE 1

#define MIN_PAGESIZE_NUMBER 16

#endif //PSU_2017_MALLOC_MALLOC_H
