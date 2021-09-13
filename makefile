CC=clang
CFLAGS=-Wall -Werror -O3
REQ=int256_t.c int256_t.h main.c
LIBS=-lm

big: $(REQ)
	find . -type f -exec touch {} +
	$(CC) $(CFLAGS) -c -o int256_t.o int256_t.c
	$(CC) $(CFLAGS) -c -o main.o main.c
	$(CC) $(CFLAGS) main.o int256_t.o -o big $(LIBS)
	./big

clean: main.o int256_t.o
	rm main.o
	rm int256_t.o
	