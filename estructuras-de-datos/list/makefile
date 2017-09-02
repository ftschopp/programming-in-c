CC 		= gcc 

all : test

test: lista.o test.c	
	cc -o test test.c lista.o

lista.o : lista.h lista.c
	cc -c lista.c
	
clean: 
	rm -rf *o test lista