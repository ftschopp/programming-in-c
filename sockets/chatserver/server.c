#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "tcpserver.h"
#include "lista.h"

void handleSigint();
void print_client_addr(struct sockaddr_in addr);
void strip_newline(char *s);
void *handleClient(void *arg);
void procesaMensaje(tcpClient *cli, char* buff); 
void sendMessageAll(tcpClient *clientOrigin, char * buff);
void listAllClients();

typedef enum
{
	CMD_QUIT,
    CMD_PING,
    CMD_HELP,
    CMD_SHOW_CLIENTS,
    CMD_UNKNOW
}commandType;

node *first;
node *last;

int main(int argc, char *argv[]) 
{
    pthread_t tid;
    tcpServer *server = malloc(sizeof(tcpServer));
    server->port = 24870;
    server->max_connection = 10;
    int connfdAccepted;
    int clientNumber = 100;
    //signal(SIGINT, handleSigint); 
    
    createTCPServer(server);

    while(1)
    {
        tcpClient *client = acceptConnection(server->listenfd);        
        printf("\nConnection Accepted %d", client->connfd);
        
        data d;
        d.client = *client;
        d.key = ++clientNumber;
        strncpy(d.name, "noname", 30);
        insert(&first, &last, d);

		pthread_create(&tid, NULL, &handleClient, (void*)client);

		sleep(1);
    }
} 

void handleSigint()
{
    printf("\n Handle sigint");
    //endServer = 1;    
}

/* Strip CRLF */
void strip_newline(char *s){
	while(*s != '\0'){
		if(*s == '\r' || *s == '\n'){
			*s = '\0';
		}
		s++;
	}
}

void *handleClient(void *arg)
{
    char buff_out[1024];
	//char buff_in[1024];
    char *buff_in = malloc(1024);
    int run = 1;
    int bytesRecved;

    if(buff_in == NULL)
    {
        perror("\nNo pude asignar memoria");
        pthread_detach(pthread_self());	
	    return NULL;
    }

    tcpClient *cli = (tcpClient *)arg;

	printf("<<ACCEPT NEW CLIENT REFERENCED BY %d \n", cli->connfd);
	print_ip_addr(cli->addr);
    
    //Agregamos el nuevo cliente a la lista
    

    /* Receive input from client */
	while(run)
    {
        bytesRecved = readSocket(cli->connfd, buff_in, 1023);

        if(bytesRecved > 0)
        {
            printf("Proceso Msj");
            procesaMensaje(cli, buff_in);        
        }
        else if(bytesRecved == 0)
        {
            //conexion remota cerrada por el cliente
            run = 0;
            printf("\nEl cliente ha cerrado la conexion");
            fflush(stdout);
            //hacer algun tratamiento diferente
        }
        else
        {
            run = 0;
            perror("Error al leer socket");
        }  

        usleep(500);      
	}

    /* Close connection */
    closeSocket(cli->connfd);
    //delete from list

    free(cli);    
    pthread_detach(pthread_self());	
	return NULL;
}

commandType getCommand(char *buff_in)
{
    char *cmd = strtok(buff_in," ");
    if(!strcmp(cmd,":QUIT")){
        return  CMD_QUIT;
    }else if(!strcmp(cmd,":PING")){
        return  CMD_PING;
    }else if(!strcmp(cmd,":HELP")){
        return CMD_HELP;
    }else if(!strcmp(cmd,":SHOW_CLIENTS")){
        return CMD_SHOW_CLIENTS;
    }
    else
    {
        return CMD_UNKNOW;
    }
}

void procesaMensaje(tcpClient *clientOrigin, char *buff_in)
{
    //ignoramos mensajes vacio
    if(!strlen(buff_in)){
        return;
    }

    strip_newline(buff_in);

    //Detectamos comandos especiales
    if(buff_in[0] == ':')
    {   
        switch(getCommand(buff_in))
        {
            case CMD_QUIT:
                printf("\nComando Quit");
                break;
            case CMD_PING:
                printf("\nComando ping");
                break;
            case CMD_HELP:
                printf("\nComando ayuda");
                break;
            case CMD_SHOW_CLIENTS:
                listAllClients();
                break;
            case CMD_UNKNOW:
                printf("\nComando desconocido");
                break;
        }            
    }
    else
    {
        printf("msj > %s\n", buff_in);
        sendMessageAll(clientOrigin, buff_in);
        //envio el mensaje a todos los clientes
    }    
}

void sendMessageAll(tcpClient* clientOrigin, char * buff_out)
{
    node* header = first;  

    while(header)
    {
        if(clientOrigin->connfd != header->info.client.connfd)
        {
            int bytesSended = writeSocket(header->info.client.connfd, buff_out, strlen(buff_out));
        }
        
        header = header->next;
    }
}

void listAllClients()
{
    node* header = first;  

    while(header)
    {
        printf("\nKey Nodo: %d", header->info.key);
        printf("\nName: %s", header->info.name);
        printf("\nConnfd: %d", header->info.client.connfd);
        printf("\nIP %d.%d.%d.%d:%d",
		header->info.client.addr.sin_addr.s_addr & 0xFF,
		(header->info.client.addr.sin_addr.s_addr & 0xFF00)>>8,
		(header->info.client.addr.sin_addr.s_addr & 0xFF0000)>>16,
		(header->info.client.addr.sin_addr.s_addr & 0xFF000000)>>24,
        ntohs((header->info.client.addr.sin_port)));
        printf("\n--");        
        header = header->next;
        
        header = header->next;
    }
}

