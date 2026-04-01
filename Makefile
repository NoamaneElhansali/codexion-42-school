NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = coders/init.c  coders/main.c  coders/monitor.c  coders/parsing.c  coders/routine.c  coders/utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) -fsanitize=address -g3 $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : clean
