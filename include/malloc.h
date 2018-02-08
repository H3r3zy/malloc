/*
** EPITECH PROJECT, 2018
** PSU_2017_malloc
** File description:
** Created by sahel.lucas-saoudi@epitech.eu,
*/

#ifndef PSU_2017_MALLOC_MALLOC_H
# define PSU_2017_MALLOC_MALLOC_H

# include	<pthread.h>
# include	<stddef.h>
# include	<stdio.h>
# include	<memory.h>
# include	<unistd.h>

extern pthread_mutex_t secure_thread;
extern unsigned long int allocated;

typedef struct s_block {
	int 	size;
	int 	free;
	struct 	s_block *previous;
	struct 	s_block *next;
} 		t_block;

extern t_block *g_list;

void 		show_alloc_mem(void);
void 		free(void *ptr);
void 		*malloc(size_t size);
void 		*realloc(void *ptr, size_t size);
void 		*calloc(size_t num, size_t size);
t_block 	*fusion_block(t_block *block);
t_block 	*split_block(t_block *block, int size);
void		move_block_data(t_block *dest, t_block *src);
t_block		*morecore(t_block *end, int size);

extern const int 	NOT_FREE;
extern const int 	FREE;

extern const int 	MIN_PAGESIZE_NUMBER;

#ifndef 	PAGESIZE
# define 	PAGESIZE		sysconf(_SC_PAGESIZE)
#endif

#ifndef 	ALIGNEMENT
# define 	ALIGNEMENT		sizeof(int *)
#endif

#ifndef		ALIGN_DOWN
# define	ALIGN_DOWN(base, size) 	((base) & -((typeof (base)) (size)))
#endif

#ifndef		ALIGN_UP
# define	ALIGN_UP(base, size)	ALIGN_DOWN((base) + (size) - 1, (size))
#endif

#ifndef 	LOCK
# define 	LOCK(x)			pthread_mutex_lock(x)
#endif

#ifndef 	UNLOCK
# define	UNLOCK(x)		pthread_mutex_unlock(x)
#endif

#ifndef 	BLOCK_SIZE
# define	BLOCK_SIZE(x)		((x) + sizeof(t_block))
#endif

#ifndef 	DATA_SIZE
# define	DATA_SIZE(x)		((x) - sizeof(t_block))
#endif

#ifndef 	VALIDBLOCK
# define 	VALIDBLOCK(block) 	(g_list && 			\
	(t_block *) (block) >= g_list && 				\
	(char *) (block) < (char *)(g_list + allocated)) ? (block) : (NULL)
#endif

#ifndef 	DATA2HEADER
# define 	DATA2HEADER(x)		((t_block *)(x) - 1)
#endif

#ifndef 	HEADER2DATA
# define 	HEADER2DATA(x) 		(char *)((x) + 1)
#endif

#ifndef 	MIN
# define 	MIN(x1, x2) 		((x1) < (x2)) ? (x1) : (x2)
#endif

#ifndef 	DATACPY
# define 	DATACPY(dest, src) memcpy(HEADER2DATA(dest), HEADER2DATA(src), \
	MIN((dest)->size, (src)->size));
#endif

#endif //PSU_2017_MALLOC_MALLOC_H