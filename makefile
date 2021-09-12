CC=clang
CFLAGS=-Wall -Werror -O3
REQ=int256_t.c int256_t.h main.c
LIBS=-lm

big: $(REQ)
	find . -type f -exec touch {} +
	$(CC) int256_t.c -c -o int256_t.o $(CFLAGS)
	$(CC) main.c -c -o main.o $(CFLAGS)
	$(CC) main.o int256_t.o -o big $(CFLAGS) $(LIBS)
	./big
