#ifndef _MQ_H
#define _MQ_H

typedef enum
{				
    MSG_NADIE,
    MSG_SERVER,
    MSG_CLIENT 
}MessageType;

typedef enum
{
    EVT_NINGUNO,
	EVT_PROCESAR, 
    EVT_OTHER,
    EVT_END
}EventType;

typedef struct
{
    int numero;
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
int borrar_mensajes(int msqid);
int showMessage(Message *);

#endif