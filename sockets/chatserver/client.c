#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include <string.h>
#include <zconf.h>
#include <stdlib.h>
#include <pthread.h>

void *sendMessage(void *sock_desc) {
    //Send some data
    while (1) {
        char message[2000];
        printf("%s","> ");
        scanf("%[^\n]%*c", message);
        fflush(stdin);

        if (send(*((int *) sock_desc), message, strlen(message) + 1, 0) < 0) {
            puts("Send failed");
        }
    }
}

void *receiveMessage(void *sock_desc) {
    while (1) {
        char server_reply[2000];
        if (recv(*((int *) sock_desc), server_reply, 2000, 0) < 0) {
            puts("recv failed");

        }
        //Receive a reply from the server
        printf("\033[32;1m %s \033[0m\n", server_reply);
    }
}

int main(int argc, char *argv[]) {
    int                sock;
    struct sockaddr_in server;
    //char               message[2000], server_reply[2000];

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family      = AF_INET;
    server.sin_port        = htons(24870);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Connect failed. Error");
        return 1;
    }

    puts("Connected to server\n");
    int *new_sock;
    new_sock = malloc(1);
    *new_sock = sock;
    //keep communicating with server

    pthread_t send_thread, receive_thread;
    pthread_create(&send_thread, NULL, sendMessage, (void *) new_sock);
    pthread_create(&receive_thread, NULL, receiveMessage, (void *) new_sock);

    pthread_join(receive_thread, NULL);
    pthread_join(send_thread, NULL);

    close(sock);
    return 0;
}

// #include <stdbool.h>
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <errno.h>
// #include <signal.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

// #include "tcpserver.h"

// void *handleMessages(void *arg);
// /****************** CLIENT CODE ****************/

// int main()
// {
//     int serverSocket;
//     char msj[250];
//     char name[50];
//     char buffer[1024];
//     serverSocket = connectTCPServer("127.0.0.1", 24870);
//     pthread_t tid;
//     printf("\nIngrese su nombre:");
//     scanf("%s", &name);

//     char commando[60];
//     strcpy(commando, ":NAME ");
//     strcat(commando, name);
//     send(serverSocket, commando, 60, 0);

//     printf("\nBienvenido! Ud. ha ingresado al chat, :FIN para salir\n");
    
//     int i = 0;
//     pthread_create(&tid, NULL, &handleMessages, (void*)serverSocket);

//     while(i ==0 || strcmp(msj, ":FIN")!=0)
//     {
//         i = 1;
//         scanf("%s",&msj);
//         send(serverSocket, commando, sizeof(msj), 0);
//         // recv(serverSocket, buffer, 1024, 0);

//         // printf("\n%s", buffer);
//         // fflush(stdout);
//     }

//     return 0;
// }


// void *handleMessages(void *arg)
// {
//     int sockfd = (int)arg;
//     char buffer[1024];
//     while(1)
//     {
//         recv(sockfd, buffer, 1024, 0);

//         printf("\n%s", buffer);
//         fflush(stdout);
//     }

//     pthread_detach(pthread_self());	
// }