setup:
	gcc --std=c99 -g -Wall -o smallsh main.c command.c shell.c

clean:
	rm smallsh