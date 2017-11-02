CC 		= gcc 

all : pienso

pienso: common.o mq.o pienso.c	
	cc -o pienso pienso.c common.o

jugador: sys_v_semaphore.o common.o jugador.c	
	cc -o jugador -lpthread jugador.c sys_v_semaphore.o common.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c

base.o : base.h
	cc -c base.h

mq.o : mq.h mq.c
	cc -c mq.c
clean: 
	rm -rf *o pienso jugador common sys_v_semaphore