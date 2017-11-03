#include "mq.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "common.h"

int createMessageQueue(int key)
{
	int msqid = msgget(creo_clave(key), 0644 | IPC_CREAT);
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

void sendMessageQueue(int msqid, MessageQueue *mq)
{
    if (msgsnd(msqid, mq, sizeof((*mq).info), 0) == -1) /* +1 for '\0' */
    {
        perror("msgsnd");
        exit(1);
    }
}		

void recvMessageQueue(int msqid, MessageQueue *mq)
{
    if (msgrcv(msqid, mq, sizeof((*mq).info), 0, 0) == -1) 
    {
        perror("msgrcv");
        exit(1);
    }
}