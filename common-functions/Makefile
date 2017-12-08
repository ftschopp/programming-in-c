CC 		= gcc 

all : test  

test : test.c common.o
	cc -o test test.c common.o 

common.o : common.c common.h
	cc -c common.c
	
clean: 
	rm -rf *o test

rebuild: 
	rm -rf *o test
	make all