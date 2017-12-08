CC 		= gcc 

all : server client

server: common.o mq.o server.c	
	cc -o server server.c common.o mq.o

client: common.o mq.o client.c	
	cc -o client client.c common.o mq.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c

mq.o : mq.h mq.c
	cc -c mq.c
clean: 
	rm -rf *o server client common sys_v_semaphore