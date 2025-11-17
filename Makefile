CC = clang
CFLAGS = -Wall -Wextra -Werror -std=c11

all: slugterm

slugterm: slugterm.c
	$(CC) $(CFLAGS) -o slugterm slugterm.c

clean:
	rm -f slugterm

.PHONY: all clean
