CC 		= gcc 

all : pthread01 join01 condition01 mutex01

pthread01: pthread01.c	
	cc -o pthread01 pthread01.c -lpthread

join01: join01.c	
	cc -o join01 join01.c -lpthread

condition01: condition01.c	
	cc -o condition01 condition01.c -lpthread

mutex01: mutex01.c	
	cc -o mutex01 mutex01.c -lpthread

clean: 
	rm -rf *o join01 mutex01 condition01 pthread 