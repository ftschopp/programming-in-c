CC 		= gcc 

all : test read

test: main.o mq.o common.o 
	cc -o main main.o mq.o common.o

read: readmq.o mq.o common.o 
	cc -o readmq readmq.o mq.o common.o

main: main.c
	cc -o main.c

readmq: readmq.c
	cc -o readmq.c

mq.o : common.o mq.h mq.c
	cc -c mq.c common.o 

common.o : common.h common.c
	cc -c common.c


clean: 
	rm -rf *o main mq common