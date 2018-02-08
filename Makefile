##
## EPITECH PROJECT, 2018
## Malloc
## File description:
## created by sahel.lucas-saoudi@epitech.eu
##

SRC	=	

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

debug:	CFLAGS += -ggdb3

debug: fclean
	gcc -D DEBUG=1 -shared -o $(NAME) -fPIC $(SRC) -lpthread

.PHONY:	all clean fclean re unaligned debug
