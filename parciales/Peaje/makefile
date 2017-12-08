CC 		= gcc 

all : auto peaje

peaje: common.o mq.o peaje.c	
	cc -o peaje peaje.c common.o mq.o

auto: common.o mq.o auto.c	
	cc -o auto auto.c common.o mq.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c

mq.o : mq.h mq.c
	cc -c mq.c
clean: 
	rm -rf *o peaje auto common sys_v_semaphore