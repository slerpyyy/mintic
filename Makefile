all: mintic

mintic: mintic.c
	$(CC) mintic.c -o mintic -O3 -Wall -Wextra -Werror

install: mintic
	cp mintic /usr/local/bin/mintic

uninstall:
	rm -f /usr/local/bin/mintic

clean:
	rm -f mintic
