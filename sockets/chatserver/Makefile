# all:
# 	$(CC) -Wall -Werror chatserver.c -O2 -lpthread -o chatserver

# clean:
# 	$(RM) -rf chatserver
CC 		= gcc 

all : server  

server : server.c tcpserver.o lista.o
	cc -o server server.c tcpserver.o lista.o -lpthread

tcpserver.o : tcpserver.c tcpserver.h
	cc -c tcpserver.c
	
lista.o : lista.h lista.c
	cc -c lista.c
	
clean: 
	rm -rf *o server

rebuild: 
	rm -rf *o server
	make all