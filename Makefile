# Makefile for src directory and libs
.phony = make all clean

all:
	make -C src

test:
	make -C src test
	tests/testing.sh

unittest:
	make -C src unittest

clean:
	rm -f *~
	make -C src clean
	rm -rf build
	rm -rf tests/minesweeper* tests/unittest*
