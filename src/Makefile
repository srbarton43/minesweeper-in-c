# Makefile for the crawler program
#
# David Kotz - April 2017, 2019, 2021
OBJS = minesweeper.o

C = 
L = 
LIBS =
LLIBS = 

CC = gcc
#LOGGING = -DAPPTEST
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(LOGGING) -I$L -I$C
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

ARGS = 

.PHONY: all clean test valgrind

all: minesweeper

# executable depends on object files
minesweeper: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $@

# object files depend on include files

test:
	./testing.sh

valgrind:
	$(VALGRIND) ./testing.sh

clean:
	rm -f minesweeper
	rm -f *~ *.o
	rm -rf *.dSYM
