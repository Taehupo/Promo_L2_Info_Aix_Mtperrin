ex1: ex1.o
	gcc ex1.o -o ex1 $$(pkg-config --libs gtk+-3.0)

ex1.o: ex1.c
	gcc -Wall -std=c99 $$(pkg-config --cflags gtk+-3.0) -c ex1.c