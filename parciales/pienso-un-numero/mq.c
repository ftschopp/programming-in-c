#include "mq.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "common.h"

int createMessageQueue(int key)
{
	int id_queue = msgget(creo_clave(key), 0600 | IPC_CREAT);
	if (id_queue == -1)
	{
		printf("Error al obtener identificador para cola mensajes\n");
		exit (-1);
	}
	return id_queue;
}

int sendMessage(int msqid, long mtype, MessageInfo info)
{
    MessageQueue mq;
    mq.mtype = mtype;
    mq.info = info;
    return msgsnd(msqid, &mq, sizeof(info), 0);
}		

int receiveMessage(int msqid, long mtype, MessageInfo* (info))
{
    return msgrcv(msqid, info, sizeof(*info), 2, 0);
}

void cleanQueue(int id_queue)
{
    if (msgctl(id_queue, IPC_RMID, NULL) == -1) 
    {
		printf("Message queue could not be deleted.\n");
        exit(EXIT_FAILURE);
    }
}

