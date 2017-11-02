CC 		= gcc 

all : cci panel

cci: sys_v_semaphore.o common.o cci.c	
	cc -o cci cci.c sys_v_semaphore.o common.o

panel: sys_v_semaphore.o common.o panel.c	
	cc -o panel panel.c sys_v_semaphore.o common.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c

common.o : common.h common.c
	cc -c common.c
	
clean: 
	rm -rf *o cci panel common sys_v_semaphore