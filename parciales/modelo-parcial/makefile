CC 		= gcc 

all : juego

juego: sys_v_semaphore.o common.o juego.c	
	cc -o juego juego.c sys_v_semaphore.o common.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c
	
clean: 
	rm -rf *o juego common sys_v_semaphore