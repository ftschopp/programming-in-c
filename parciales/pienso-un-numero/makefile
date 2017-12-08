CC 		= gcc 

all : pienso jugador

pienso: common.o mq.o pienso.c	
	cc -o pienso pienso.c common.o mq.o

jugador: common.o mq.o jugador.c	
	cc -o jugador jugador.c common.o mq.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c

mq.o : mq.h mq.c
	cc -c mq.c
clean: 
	rm -rf *o pienso jugador common sys_v_semaphore