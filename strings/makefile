CC = gcc
AS = as
CFLAGS = -Wall -Wextra -std=c99 -g

SRCS = main.c libstr.h b64.c hamming.s
OBJS = main.o b64.o hamming.o

.PHONY: all clean run debug

all: part1.out

part1.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -no-pie

main.o: main.c libstr.h
	$(CC) $(CFLAGS) -c -o $@ main.c

b64.o: b64.c libstr.h
	$(CC) $(CFLAGS) -c -o $@ b64.c

hamming.o: hamming.s
	$(AS) -g hamming.s -o hamming.o

run: part1.out
	./part1.out

debug: part1.out
	gdb -ex run --args ./part1.out

clean:
	rm -f part1.out $(OBJS)
