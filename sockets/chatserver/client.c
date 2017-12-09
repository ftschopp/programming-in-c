#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "tcpserver.h"


void validaParametrosInput(int argc, char *argv[]);
void *handleMessages(void *arg);

/****************** CLIENT CODE ****************/

int main(int argc, char *argv[]) 
{
    validaParametrosInput(argc, argv);
    char ipServer[20];
    int puerto;
    strcpy(ipServer, argv[1]);
    puerto = atoi(argv[2]);
    int serverSocket;
    char msj[250];
    char name[50];
    char buffer[1024];
    serverSocket = connectTCPServer(ipServer, puerto);
    pthread_t tid;
    printf("\nIngrese su nombre:");
    fgets(name, 50, stdin);

    char commando[60];
    strcpy(commando, ":NAME ");
    strcat(commando, name);
    send(serverSocket, commando, 60, 0);

    printf("\nBienvenido! Ud. ha ingresado al chat, :FIN para salir\n");
    
    int i = 0;
    pthread_create(&tid, NULL, &handleMessages, (void*)serverSocket);

    while(i ==0 || strcmp(msj, ":FIN")!=0)
    {
        i = 1;
        fgets(msj, 250, stdin);
        fflush(stdin);
        send(serverSocket, msj, strlen(msj), 0);
        // recv(serverSocket, buffer, 1024, 0);

        // printf("\n%s", buffer);
        // fflush(stdout);
    }

    return 0;
}

//Declaracion de funciones

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("\nDebe recibir el número de ip y puerto\n\n");
        exit(1);
    }
}

void *handleMessages(void *arg)
{
    int sockfd = (int)arg;
    char buffer[1024];
    while(1)
    {
        recv(sockfd, buffer, 1024, 0);

        printf("\n%s", buffer);
        fflush(stdout);
    }

    pthread_detach(pthread_self());	
}