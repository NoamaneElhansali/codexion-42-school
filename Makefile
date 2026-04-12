NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = coders/init.c  coders/main.c  coders/monitor.c  coders/parsing.c  coders/routine.c  coders/utils.c coders/fifo.c coders/edf.c coders/utils1.c coders/actions.c coders/utils2.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

debug:
	$(CC) $(CFLAGS)  -fsanitize=thread -g $(SRC) -o $(NAME)
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re-d: fclean debug
re: fclean all

.PHONY : clean
