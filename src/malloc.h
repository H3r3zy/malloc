/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef PSU_2017_MALLOC_MALLOC_H
#define PSU_2017_MALLOC_MALLOC_H

#include <pthread.h>

pthread_mutex_t thread_safe = PTHREAD_MUTEX_INITIALIZER;

unsigned long int allocated = 0;

typedef struct s_block {
	int size;
	int free;
	struct s_block *previous;
	struct s_block *next;
} t_block;

t_block *g_list = NULL;

void show_alloc_mem(void);

void free(void *ptr);

void *malloc(size_t size);

void *realloc(void *ptr, size_t size);

void *calloc(size_t num, size_t size);

const int NOT_FREE = 0;
const int FREE = 1;

#define PAGESIZE                        sysconf(_SC_PAGESIZE)
const int MIN_PAGESIZE_NUMBER = 32;

#define ALIGNEMENT sizeof(int *)

#ifndef ALIGN_DOWN
	#define ALIGN_DOWN(base, size) ((base) & -((typeof (base)) (size)))
#endif
#ifndef ALIGN_UP
	#define ALIGN_UP(base, size) ALIGN_DOWN((base) + (size) - 1, (size))
#endif

#define LOCK(x) pthread_mutex_lock(x)
#define UNLOCK(x) pthread_mutex_unlock(x)

#define BLOCK_SIZE(x) ((x) + sizeof(t_block))
#define DATA_SIZE(x) ((x) - sizeof(t_block))

#define VALIDBLOCK(block) (g_list && (t_block *) (block) >= g_list && \
(char *) (block) < (char *)(g_list + allocated)) ? (block) : (NULL)

#define DATA2HEADER(x) ((t_block *)(x) - 1)
#define HEADER2DATA(x) (char *)((x) + 1)

#define MIN(x1, x2) ((x1) < (x2)) ? (x1) : (x2)

#define DATACPY(dest, src) memcpy(HEADER2DATA(dest), HEADER2DATA(src), \
MIN((dest)->size, (src)->size));

#endif //PSU_2017_MALLOC_MALLOC_H