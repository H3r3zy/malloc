##
## EPITECH PROJECT, 2018
## Malloc
## File description:
## created by sahel.lucas-saoudi@epitech.eu
##

SRC	=	src/malloc.c

OBJ	=	$(SRC:.c=.o)

NAME	=	libmy_malloc.so

CFLAGS	=	-W -Wall -Wextra -I src

$(NAME):
	gcc -shared -o $(NAME) -fPIC $(SRC)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY:	all clean fclean re
