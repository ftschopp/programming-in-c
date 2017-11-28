CC 		= gcc 

all : server client  

server : server.c 
	cc -o server server.c

client : client.c 
	cc -o client client.c

clean: 
	rm -rf *o server client

rebuild: 
	rm -rf *o test
	make all