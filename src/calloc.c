/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by martin.januario@epitech.eu,
*/

#include "malloc.h"

void *calloc(size_t num, size_t size)
{
	void *ptr = malloc(num * size);

	if (ptr) {
		LOCK(&secure_thread);
		memset(ptr, 0, num * size);
		UNLOCK(&secure_thread);
	}
	return (ptr);
}
