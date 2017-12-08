#ifndef _MQ_H
#define _MQ_H

typedef enum
{				
    MSG_NADIE,
    MSG_AUTOS,
    MSG_VIAS 
}MessageType;

typedef enum
{
    EVT_NINGUNO,
	EVT_VEHICULOS, 
    EVT_FINALIZAR
}EventType;

typedef struct
{
    int cantidad;
}MessageInfo;

typedef struct
{
    long to;
    long from;
    EventType event;
    MessageInfo info;
}Message;

int createMessageQueue(int key);
void cleanMessageQueue(int msqid);
void sendMessageQueue(int msqid, Message *msg);
void recvMessageQueue(int msqid, long to, Message *msg);


#endif