#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

typedef struct{
    int listenfd;
    int port;
    int max_connection;
    struct sockaddr_in addr;
}tcpServer;

void createTCPServer(tcpServer *server);

//Accept receive listen file descriptor 
int acceptConnection(int listenfd);
//Lee del socket
int readSocket(int sockfd, char *buff_in, int len);
//Escribe en el socket
int writeSocket(int sockfd, char *buff_out, int len);
//Cierra el socket
void closeSocket(int sockfd);
//Cierra un lado del socket
void shutdownSocket(int sockfd);
//Imprimimos la direccion ip
void print_ip_addr(struct sockaddr_in addr);
//connect to server
int connectTCPServer(char *hostname, int port);

#endif