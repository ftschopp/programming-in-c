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
int sendMessage(int id_queue, long mtype, MessageInfo info);
int receiveMessage(int id_queue, long mtype, MessageInfo* info);
void cleanQueue(int id_queue);

#endif