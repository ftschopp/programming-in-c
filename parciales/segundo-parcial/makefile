CC 		= gcc 

all : bomba artificiero

bomba: common.o mq.o bomba.c	
	cc -o bomba bomba.c common.o mq.o

artificiero: common.o mq.o artificiero.c	
	cc -o artificiero artificiero.c common.o mq.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c

mq.o : mq.h mq.c
	cc -c mq.c
clean: 
	rm -rf *o bomba artificiero common sys_v_semaphore