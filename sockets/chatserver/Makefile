# all:
# 	$(CC) -Wall -Werror chatserver.c -O2 -lpthread -o chatserver

# clean:
# 	$(RM) -rf chatserver
CC 		= gcc 

all : server client 

server : server.c tcpserver.o lista.o
	cc -o server server.c tcpserver.o lista.o -lpthread

client : client.c tcpserver.o 
	cc -o client client.c tcpserver.o

tcpserver.o : tcpserver.c tcpserver.h
	cc -c tcpserver.c
	
lista.o : lista.h lista.c
	cc -c lista.c
	
clean: 
	rm -rf *o server client

rebuild: 
	rm -rf *o server client
	make all