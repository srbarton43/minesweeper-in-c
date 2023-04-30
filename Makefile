# Makefile for src directory and libs
.phony = make all clean

all:
	make -C src

clean:
	rm -f *~
	make -C clean src
