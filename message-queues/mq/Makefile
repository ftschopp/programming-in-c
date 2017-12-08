CC 		= gcc 

all : kirk spock

kirk: kirk.c mq.o common.o 
	cc -o kirk kirk.c mq.o common.o

spock: spock.c mq.o common.o 
	cc -o spock spock.c mq.o common.o

mq.o : common.o mq.h mq.c
	cc -c mq.c 

common.o : common.h common.c
	cc -c common.c

clean: 
	rm -rf *o kirk spock