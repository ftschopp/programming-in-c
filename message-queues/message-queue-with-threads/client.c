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


typedef struct
{
    int msqid;
    int nro;
    int cantHilos;						
}t_hilo;

void validaParametrosInput(int argc, char *argv[]);
void *ProcesamientoHilo(void *threadParam);

int main(int argc, char *argv[])
{
    random_init();
    validaParametrosInput(argc, argv);

    int cantHilos = atoi(argv[1]);
    
    int msqid = createMessageQueue(ID_MQ);
    int i = 0;

    t_hilo *datos_thread;
    
    pthread_t *idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantHilos);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
    pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
    
    datos_thread = (t_hilo*) malloc(sizeof(t_hilo)*cantHilos);
	
	for(i=0; i < cantHilos; i++)
	{
        datos_thread[i].msqid = msqid;
		datos_thread[i].nro = i+1;		
        datos_thread[i].cantHilos = cantHilos;		
	
		pthread_create (&idHilo[i], &atributos, ProcesamientoHilo, &datos_thread[i]);
	}

	for(i=0; i < cantHilos; i++)
	{
		pthread_join (idHilo[i], NULL);
        // printf("\nRecibi de la via %d %d liberaciones",i+1, datos_thread[i].cantidadLiberaciones);
        //totalLiberaciones+= datos_thread[i].cantidadLiberaciones;
    }			
    		
	// printf("\nTOTAL %d\n", totalLiberaciones);

    return 0;
}


//Declaracion de funciones

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir la cantidad de hilos\n\n");
        exit(1);
    }
    
    int val = atoi(argv[1]);

    if (val < 1 || val > CANT_MAX_HILOS)
    {
        printf("\nLa cantidad de hilos no cumple con los permitido.\n\n");
        exit(1);
    }
}



void *ProcesamientoHilo(void *threadParam)
{
    t_hilo* hiloParam = (t_hilo*)threadParam;
    
    int nro = hiloParam->nro;
    int msqid = hiloParam->msqid;
    int cantHilos = hiloParam->cantHilos;
   
    random_init();
    printf("\n Hilo %d", nro);
    int done = 0;


    while(done == 0)
    {
        //int hiloElegido = random_generate(1, cantHilos);

        Message msg;
        msg.to = MSG_SERVER;
        msg.from = MSG_CLIENT + nro;
        msg.event = EVT_PROCESAR;
        msg.info.numero = random_generate(200, 300);

        printf("\nSoy Hilo %d y envio", nro);
        showMessage(&msg);
        sendMessageQueue(msqid, &msg);
        
        Message rta;

        recvMessageQueue(msqid, MSG_CLIENT + nro, &rta);
        printf("\nRecibi.... ");
        showMessage(&rta);

        switch(rta.event)
        {
            case EVT_END:
                done = 1;
                break;
        }

        usleep(100 * 1000);
    }

    pthread_exit ((void *)"Listo");
}