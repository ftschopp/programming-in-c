#ifndef _MQ_H
#define _MQ_H

typedef enum
{				
	MSG_PIENSO = 1,		
	MSG_JUGADOR = 2	
}MessageType;

typedef enum
{
    EVT_NINGUNO = 0,
	EVT_NUMERO = 1, 
	EVT_ACERTO = 2, 
    EVT_NO_ACERTO = 3,
    EVT_FINALIZAR = 4
}Event;

typedef struct
{
    int nro_jugador;
    int nro_pensado;
    Event event;
}MessageInfo;

typedef struct
{
    long mtype;
    MessageInfo info;
}MessageQueue;

int createMessageQueue(int key);
void cleanMessageQueue(int msqid);
void sendMessageQueue(int msqid, MessageQueue *mq);
void recvMessageQueue(int msqid, MessageQueue *mq);

void showMessageQueue(MessageQueue *mq);

#endif