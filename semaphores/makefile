CC 		= gcc 

all : test1 test2

test1: sys_v_semaphore.o test1.c	
	cc -o test1 test1.c sys_v_semaphore.o

test2: sys_v_semaphore.o test2.c	
	cc -o test2 test2.c sys_v_semaphore.o

sys_v_semaphore.o : sys_v_semaphore.h sys_v_semaphore.c
	cc -c sys_v_semaphore.c
	
clean: 
	rm -rf *o printer printer_prompt