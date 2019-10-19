all: tic

tic: tic.c
	$(CC) tic.c -o tic -O3 -Wall -Wextra -Werror