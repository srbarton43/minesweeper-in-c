# Makefile for minesweeper in c

C = 
L = 
LIBS =
LLIBS = 

#LOGGING = -DAPPTEST
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(LOGGING) -I$L -I$C
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

#                 Targets                #
NATIVETARGET=../build/native/minesweeper
RPITARGET=../build/ARM-aarch64/minesweeper

ARGS = 

.PHONY: all clean test valgrind debug unittest

all: $(NATIVETARGET) $(RPITARGET) 
 

# **************************************************** #
#                  Native Target                       #
# **************************************************** #
CC = gcc
ODIR = ../build/native/obj
_OBJ = minesweeper.o board.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	$(shell mkdir -p $(ODIR))
	$(CC) -c -o $@ $< $(CFLAGS)

$(NATIVETARGET): $(OBJ) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJ) $(LLIBS) -o $@

# object files depend on include files

# **************************************************** #
#                Raspberry Pi Target                   #
# **************************************************** #
RPICC = aarch64-linux-gnu-gcc
RODIR=../build/ARM-aarch64/obj
_ROBJ = board.o minesweeper.o
ROBJ = $(patsubst %,$(RODIR)/%,$(_ROBJ))

$(RODIR)/%.o: %.c
	$(shell mkdir -p $(RODIR))
	$(RPICC) -c -o $@ $< $(CFLAGS) 

$(RPITARGET): $(ROBJ)
	$(RPICC) -o $@ $^ $(CFLAGS)

# **************************************************** #
#                 Testing & Debugging                  #
# **************************************************** #
TESTDIR=../tests

debug: board.c $(TESTDIR)/minesweeper.o
	$(CC) $(CFLAGS) -DDEBUG $(TESTDIR)/minesweeper.o board.c $(LLIBS) -o $(TESTDIR)/minesweeper

$(TESTDIR)/minesweeper.o: minesweeper.c
	$(CC) $(CFLAGS) -c -o $@ $<

unittest: board.c
	$(CC) $(CFLAGS) -DDEBUG -DUNIT_TEST board.c -o $(TESTDIR)/$@

test:
	make unittest
	make debug

valgrind:
	$(VALGRIND) ./testing.sh

clean:
	rm -f minesweeper
	rm -f minesweeper-rpi
	rm -f unittest
	rm -f *~ *.o
	rm -rf *.dSYM
