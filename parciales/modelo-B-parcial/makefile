CC 		= gcc 

all : peaje autos

peaje: sys_v_semaphore.o common.o peaje.c	
	cc -o peaje peaje.c sys_v_semaphore.o common.o

autos: sys_v_semaphore.o common.o autos.c	
	cc -o autos autos.c sys_v_semaphore.o common.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c
	
clean: 
	rm -rf *o peaje autos common sys_v_semaphore