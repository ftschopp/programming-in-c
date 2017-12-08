#ifndef _MQ_H
#define _MQ_H

typedef enum
{				
    MSG_NADIE,
    MSG_BOMBA,
    MSG_ARTIFICIEROS 
}MessageType;

typedef enum
{
    EVT_NINGUNO,
	EVT_CABLE, 
    EVT_DESACTIVADA,
    EVT_EXPLOTO,
    EVT_OTRO_INTENTO,
    EVT_FINALIZAR
}EventType;

typedef struct
{
    int cable;
    int intentos;
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


#endif