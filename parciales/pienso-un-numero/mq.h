#ifndef _MQ_H
#define _MQ_H

typedef enum
{
	MSG_NADIE,				
	MSG_PIENSO,		
	MSG_JUGADOR	
}TipoMensaje;

typedef enum
{
	EVT_NINGUNO,
	EVT_NUMERO, 
	EVT_ACERTO, 
    EVT_NO_ACERTO,
    EVT_FINALIZAR
}Eventos;

typedef struct
{
    int nro_jugador;
    int nro_pensado;
    TipoMensaje tipo;
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

#endif