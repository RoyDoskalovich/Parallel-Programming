CC = gcc
AS = as
CFLAGS = -Wall -Wextra -std=c99 -g -msse4.2

SRCS = main.c formulas.h formula1.c formula2.s
OBJS = main.o formula1.o formula2.o

.PHONY: all clean run debug

all: part2.out


part2.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -no-pie -lm

main.o: main.c formulas.h
	$(CC) $(CFLAGS) -c -o $@ main.c

formula1.o: formula1.c formulas.h
	$(CC) $(CFLAGS) -c -o $@ formula1.c

formula2.o: formula2.s
	$(AS) -g formula2.s -o formula2.o

run: part2.out
	./part2.out

debug: a.out
	gdb run --args ./part2.out

clean:
	rm  -f  part2.out  *.o
