##
## EPITECH PROJECT, 2018
## Malloc
## File description:
## created by sahel.lucas-saoudi@epitech.eu
##

SRC	=	src/malloc_alligned.c

SRC2	=	src/malloc.c

OBJ	=	$(SRC:.c=.o)

NAME	=	libmy_malloc.so

CFLAGS	=	-W -Wall -Wextra -I src

$(NAME):
	gcc -shared -o $(NAME) -fPIC $(SRC) -lpthread

all: $(NAME)

unaligned:
	gcc -shared -o $(NAME) -fPIC $(SRC2) -lpthread

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY:	all clean fclean re