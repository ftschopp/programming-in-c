#include "mq.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "common.h"

int createMessageQueue(int key)
{
	int msqid = msgget(creo_clave(key), 0600 | IPC_CREAT);
	if (msqid == -1)
	{
		printf("Error al obtener identificador para cola mensajes\n");
		exit (-1);
	}
	return msqid;
}

void cleanMessageQueue(int msqid)
{
    if (msgctl(msqid, IPC_RMID, NULL) == -1) 
    {
		printf("Message queue could not be deleted.\n");
        exit(EXIT_FAILURE);
    }
}

void sendMessageQueue(int msqid, Message *msg)
{
    if(msgsnd (msqid, (struct msgbuf *)msg, sizeof(msg->from)+sizeof(msg->event)+sizeof(msg->info), 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }
}		

void recvMessageQueue(int msqid, long to, Message *msg)
{
    if (msgrcv(msqid, (struct msgbuf *)msg, sizeof(msg->from)+sizeof(msg->event)+sizeof(msg->info), to, 0) == -1) 
    {
        perror("msgrcv");
        exit(1);
    }
}