CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: progc progcs

progc: progc.c
	$(CC) $(CFLAGS) -o progc progc.c

progcs: progcs.c
	$(CC) $(CFLAGS) -o progcs progcs.c

clean:
	rm -f progc progcs
