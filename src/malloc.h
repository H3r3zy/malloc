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

typedef struct s_block {
	int size;
	int free;
	struct s_block *previous;
	struct s_block *next;
} t_block;

#define NOT_FREE 			0
#define FREE 				1

#define PAGESIZE 			sysconf(_SC_PAGESIZE)
#define MIN_PAGESIZE_NUMBER 		16

#define ALIGNEMENT 			sizeof(int *)

#ifndef ALIGN_DOWN
	#define ALIGN_DOWN(base, size)  ((base) & -((typeof (base)) (size)))
#endif
#ifndef ALIGN_UP
	#define ALIGN_UP(base, size)    ALIGN_DOWN((base) + (size) - 1, (size))
#endif

#define LOCK(x) 			pthread_mutex_lock(x)
#define UNLOCK(x) 			pthread_mutex_unlock(x)

#define BLOCK_SIZE(x) 			((x) + sizeof(t_block))
#define DATA_SIZE(x) 			((x) - sizeof(t_block))

#define VALIDBLOCK(block)		(g_list && \
					(t_block *) (block) >= g_list && \
					(void *) (block) < sbrk(0)) \
					? (block) : (NULL)

#define DATA2HEADER(x) 			((t_block *)(x) - 1)
#define HEADER2DATA(x) 			(char *)((x) + 1)

#define MIN(x1, x2) 			((x1) < (x2)) ? (x1) : (x2)

#define DATACPY(dest, src)		memcpy(HEADER2DATA(dest), \
					HEADER2DATA(src), \
					MIN((dest)->size, (src)->size));

#endif //PSU_2017_MALLOC_MALLOC_H

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
