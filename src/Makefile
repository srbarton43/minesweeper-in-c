# Makefile for the crawler program
#
# David Kotz - April 2017, 2019, 2021
OBJS = minesweeper.o board.o
RPIOBJS= minesweeper-rpi.o board-rpi.o

C = 
L = 
LIBS =
LLIBS = 

CC = gcc
#LOGGING = -DAPPTEST
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(LOGGING) -I$L -I$C
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

RPICC = aarch64-linux-gnu-gcc

ARGS = 

.PHONY: all clean test valgrind debug unittest

all: minesweeper minesweeper-rpi

rpi: minesweeper-rpi $(RPIOBJS)

minesweeper: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $@

board.o: board.h

debug: board.c minesweeper.o
	$(CC) $(CFLAGS) -DDEBUG minesweeper.o board.c $(LLIBS) -o minesweeper

unittest: board.c
	$(CC) $(CFLAGS) -DDEBUG -DUNIT_TEST board.c -o $@
# object files depend on include files

	
# for rpi
minesweeper-rpi: $(RPIOBJS) $(LLIBS)
	$(RPICC) $(CFLAGS) $(RPIOBJS) $(LLIBS) -o $@

minesweeper-rpi.o: minesweeper.c
	$(RPICC) $(CFLAGS)  -c -o $@ $<
board-rpi.o: board.c
	$(RPICC) $(CFLAGS) -c -o $@ $<

test:
	make debug
	./testing.sh

valgrind:
	$(VALGRIND) ./testing.sh

clean:
	rm -f minesweeper
	rm -f unittest
	rm -f *~ *.o
	rm -rf *.dSYM
