# Makefile for src directory and libs
.phony = make all clean

all:
	make -C minesweeper

test:
	make -C minesweeper test
	tests/testing.sh

unittest:
	make -C src unittest

clean:
	rm -f *~
	make -C minesweeper clean
	rm -rf build
	rm -rf tests/minesweeper* tests/unittest*
