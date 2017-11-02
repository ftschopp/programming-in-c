CC 		= g++ 

all : autos peaje

peaje : peaje.c funciones.h definiciones.h funciones.o
	cc -o peaje peaje.c funciones.o -pthread

autos : autos.c funciones.h definiciones.h funciones.o
	cc -o autos autos.c funciones.o

funciones.o : funciones.c funciones.h
	cc -c funciones.c
	
clean: 
	rm -rf *o autos peaje *~
