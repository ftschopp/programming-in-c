CC 		= gcc 

all : test

test: stack.o test.c	
	cc -o test test.c stack.o

stack.o : stack.h stack.c
	cc -c stack.c
	
clean: 
	rm -rf *o test stack