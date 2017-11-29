#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include "tcpserver.h"


void createTCPServer(tcpServer *server)
{
    /* Socket settings */
	server->listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	server->addr.sin_family = AF_INET;
	server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server->addr.sin_port = htons(server->port); 
	printf("\nCreating server...");
    /* Bind */
	if(bind(server->listenfd, (struct sockaddr*)&server->addr , sizeof(server->addr)) < 0){
		perror("Socket binding failed");
	}
	printf("\nBinding OK");

    /* Listen */
	if(listen(server->listenfd, server->max_connection) < 0){
		perror("Socket listening failed");
	}
	printf("\nListening on %d", server->port);
	fflush(stdout);
}

tcpClient* acceptConnection(int listenfd)
{
	tcpClient* client = malloc(sizeof(tcpClient));
    socklen_t clilen = sizeof(client->addr);
	int connfd = accept(listenfd, (struct sockaddr*)&client->addr, &clilen); 
	
	if(connfd < 0)
	{
		perror("accept error");
		return NULL;
	}
	client->connfd = connfd; 

	return client;
}

//Imprime la IP 
void print_ip_addr(struct sockaddr_in addr)
{
	printf("\n%d.%d.%d.%d",
		addr.sin_addr.s_addr & 0xFF,
		(addr.sin_addr.s_addr & 0xFF00)>>8,
		(addr.sin_addr.s_addr & 0xFF0000)>>16,
		(addr.sin_addr.s_addr & 0xFF000000)>>24);
}

//Lee datos del socket
int readSocket(int sockfd, char *buff_in, int len)
{	
	int bytesRead = recv(sockfd, buff_in, len, 0);
	//printf("\nBytes Leidosss %d", bytesRead);

	if(bytesRead < 0){
		perror("ReadSocket error");
	}
	else
	{
		buff_in[bytesRead] = '\0';	
	}

	return  bytesRead;
}

//Escribe en el socket
int writeSocket(int sockfd, char *buff_out, int len)
{
	int bytesSended = send(sockfd, buff_out, len, 0);	

	if(bytesSended < 0){
		perror("SendSocket error");
	}	

	return  bytesSended;
}

//Cierra ambos lados del socket tanto el cliente como el server
void closeSocket(int sockfd)
{
	close(sockfd);
}

//Cierra un solo lado del socket.
void shutdownSocket(int sockfd)
{
	shutdown(sockfd, SHUT_WR);
}


tcpClient connectTCPServer(char *hostname, int port)
{
	int sockfd;
	tcpClient client;
	struct hostent *server;
   
	/* Create a socket point */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	server = gethostbyname(hostname);

	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	client.addr.sin_family = AF_INET;
	client.addr.sin_port = htons(port);
	client.connfd = sockfd;

	/* Now connect to the server */
	if (connect(sockfd, (struct sockaddr*)&client.addr, sizeof(client.addr)) < 0) {
		perror("ERROR connecting");
		exit(1);
	}

	return client;
}