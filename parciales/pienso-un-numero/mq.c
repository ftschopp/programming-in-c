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
    if (msgsnd(msqid, mq, sizeof((*mq).info), IPC_NOWAIT) == -1) /* +1 for '\0' */
    {
        perror("msgsnd");
        exit(1);
    }
}		

void recvMessageQueue(int msqid, MessageQueue *mq)
{
    if (msgrcv(msqid, mq, sizeof((*mq).info), (*mq).mtype, 0) == -1) 
    {
        perror("msgrcv");
        exit(1);
    }
}

void showMessageQueue(MessageQueue *mq)
{
    printf("mtype %ld", mq->mtype);
    printf(" info: NroJugador: %d NroPensado:%d Evento %d\n", mq->info.nro_jugador, mq->info.nro_pensado, mq->info.event);
}