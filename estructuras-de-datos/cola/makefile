CC 		= gcc 

all : test

test: queue.o test.c	
	cc -o test test.c queue.o

queue.o : queue.h queue.c
	cc -c queue.c
	
clean: 
	rm -rf *o test queue