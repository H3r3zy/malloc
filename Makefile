##
## EPITECH PROJECT, 2018
## Malloc
## File description:
## created by sahel.lucas-saoudi@epitech.eu
##

SRC		=	src/malloc_alligned.c		\
			src/realloc.c			\
			src/free.c			\
			src/show_alloc_mem.c		\
			src/calloc.c			\
			src/manage_block.c

OBJ		=	$(SRC:.c=.o)

NAME		=	libmy_malloc.so

CFLAGS		=	-W -Wall -Wextra -fPIC -I include/

$(NAME):		$(OBJ)
			gcc -shared -o $(NAME) $(OBJ)

all:			$(NAME)

clean:
			rm -rf $(OBJ)

fclean:			clean
			rm -rf $(NAME)

re:			fclean all

debug:			CFLAGS += -ggdb3

debug:			fclean
			gcc -D DEBUG=1 -shared -o $(NAME) -fPIC $(SRC) -lpthread

.PHONY:			all clean fclean re debug
