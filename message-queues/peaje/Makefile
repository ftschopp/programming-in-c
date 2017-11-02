CC 		= g++ 

all : peaje  

peaje : peaje.c sys_v_semaphore.o common.o
	cc -o peaje -lpthread peaje.c sys_v_semaphore.o common.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c
	
clean: 
	rm -rf *o peaje