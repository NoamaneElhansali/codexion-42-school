NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = coders/init.c  coders/main.c  coders/monitor.c  coders/parsing.c  coders/routine.c  coders/utils.c coders/fifo.c coders/edf.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

debug:
	$(CC) $(CFLAGS) -fsanitize=address -g3 $(SRC) -o $(NAME)
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : clean
