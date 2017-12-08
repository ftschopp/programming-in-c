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
#define CANT_MAX_HILOS 999

void finalizoClients();
void validaParametrosInput(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    random_init();
    validaParametrosInput(argc, argv);

    //int cantHilos = atoi(argv[1]);
    
    //int hilo = random_generate(1, cantHilos);
    
    int msqid = createMessageQueue(ID_MQ);
    int done = 0;

    //cleanMessageQueue(msqid);
    int res = borrar_mensajes(msqid);
 
    while(done == 0)
    {
        Message msg;
        Message mRta;

        recvMessageQueue(msqid, MSG_SERVER, &msg);
        printf("\n ****************Recibido********************");
        showMessage(&msg);
        printf("\n Presione una tecla para procesar el msg...");
        getchar();

        switch(msg.event)
        {
            case EVT_PROCESAR: 
                mRta.to = msg.from;
                mRta.from = MSG_SERVER;

                mRta.info.numero =  msg.info.numero * 2;
                //
                printf("\n Envio Respuesta ");
                showMessage(&mRta);
                sendMessageQueue(msqid, &mRta);
                break;
            case EVT_OTHER:
                printf("\n Recibi OTHER EVENT");
                break;
            case EVT_END:
                printf("\n TERMINO");
                done = 1;
                finalizoClients();
                break;
        }
    }
    
    return 0;
}

void finalizoClients()
{

}

//Declaracion de funciones

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir la cantidad de hilos a atender\n\n");
        exit(1);
    }
    
    int val = atoi(argv[1]);

    if (val < 1 || val > CANT_MAX_HILOS)
    {
        printf("\nLa cantidad de hilos no cumple con los permitido.\n\n");
        exit(1);
    }
}

