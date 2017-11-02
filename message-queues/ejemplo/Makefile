CC 		= g++ 

all : repositor supermercado 

supermercado : supermercado.c funciones.h definiciones.h funciones.o
	cc -o supermercado supermercado.c funciones.o 

repositor : repositor.c funciones.h definiciones.h funciones.o
	cc -o repositor repositor.c funciones.o

funciones.o : funciones.c funciones.h
	cc -c funciones.c
	
clean: 
	rm -rf *o supermercado repositor