#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "mq.h"
#include "common.h"

#define ID_MQ 2432
#define CANT_MAX_CABLES 999

void validaParametrosInput(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    random_init();
    validaParametrosInput(argc, argv);

    int cantCables = atoi(argv[1]);
    int intentos = cantCables;
    int desactiva = random_generate(1, cantCables);
    int artificieroResponsable = 0;
    int msqid = createMessageQueue(ID_MQ);
    int done = 0;

    //cleanMessageQueue(msqid);
    int res = borrar_mensajes(msqid);
 
    while(done == 0 && intentos > 0)
    {
        Message msg;
        Message mRta;

        recvMessageQueue(msqid, MSG_BOMBA, &msg);

        //printf("\n viendo");
        //getchar();
        switch(msg.event)
        {
            case EVT_NINGUNO: 
            case EVT_DESACTIVADA: 
            case EVT_EXPLOTO:
            case EVT_OTRO_INTENTO:
            case EVT_FINALIZAR:
                //no hacen nada estos eventos..
                break; 
            case EVT_CABLE: 

                mRta.to = msg.from;
                mRta.from = MSG_BOMBA;
               
                if(msg.info.cable == desactiva)
                {
                    mRta.event = EVT_DESACTIVADA;
                    done = 1;
                    artificieroResponsable = msg.from;
                }
                else
                {
                    intentos--;

                    if(intentos > 0)
                    {
                        mRta.event = EVT_OTRO_INTENTO;
                    }
                    else
                    {
                        mRta.event = EVT_EXPLOTO;
                        artificieroResponsable = msg.from;
                    }
                }

                //en la respuesta envio la cantidad de intentos consumidas
                mRta.info.intentos = cantCables - intentos;
                
                sendMessageQueue(msqid, &mRta);
                break;
        }
    }

    printf("\n Intentos ocurridos %d ", cantCables - intentos);
    if(done == 0)
    {
        printf("\nBomba Desactivada por Artificiero %d ", artificieroResponsable - 1);
    }
    else
    {
        printf("\nBomba Explotada por Artificiero %d", artificieroResponsable - 1);
    }

    return 0;
}


//Declaracion de funciones

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir la cantidad de cables de la bomba\n\n");
        exit(1);
    }
    
    int val = atoi(argv[1]);

    if (val < 1 || val > CANT_MAX_CABLES)
    {
        printf("\nLa cantidad de cables de la bomba no cumple con los permitido.\n\n");
        exit(1);
    }
}

