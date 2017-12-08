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
#define CANT_MAX_VIAS 5
#define MIN_AUTOS 1
#define MAX_AUTOS 5
#define MIN_ESPERA 100
#define MAX_ESPERA 500


void validaParametrosInput(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    validaParametrosInput(argc, argv);
    int cantidadVias = atoi(argv[1]);
    int cantidadAutos = atoi(argv[2]);
    random_init();
    
    int msqid = createMessageQueue(2487);

    while(cantidadAutos > 0)
    {
        int autosGenerados = random_generate(MIN_AUTOS, MAX_AUTOS);

        if(autosGenerados > cantidadAutos)
        {
            autosGenerados = cantidadAutos;
        }

        int viaActual = random_generate(1, cantidadVias);
        
        MessageInfo info;
        info.cantidad = autosGenerados;

        Message msg;
        msg.to = MSG_VIAS + viaActual;
        msg.from = MSG_AUTOS;
        msg.event = EVT_VEHICULOS;
        msg.info = info;

        printf("\n Msqid %d to %ld Autos Generados %d VIA ELEGIDA %d", msqid, msg.to, msg.info.cantidad, viaActual);
        sendMessageQueue(msqid, &msg);

        cantidadAutos -= autosGenerados;

        
        int espera = random_generate(MIN_ESPERA, MAX_ESPERA);
        
        printf("\n Espero %d Cantidad disponible %d", espera, cantidadAutos);
        usleep(espera * 1000);
    }

    int i = 1;
    for(i = 1; i <= cantidadVias; i++)
	{
        Message msgFin;
        msgFin.event = EVT_FINALIZAR;
        msgFin.from = MSG_AUTOS;
        msgFin.to = MSG_VIAS + i;
		sendMessageQueue(msqid, &msgFin);
	}

    
    return 0;
}


//Declaracion de funciones

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("\nDebe recibir el número de vias y la cantidad de autos a generar\n\n");
        exit(1);
    }
    
    int val = atoi(argv[1]);

    if (val < 1 || val > CANT_MAX_VIAS)
    {
        printf("\nEl nro de vias no cumple con los permitidos\n\n");
        exit(1);
    }
}

