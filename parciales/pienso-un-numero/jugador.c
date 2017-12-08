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

#define CANT_MAX_JUGADORES 10
#define MIN_NUM 1
#define MAX_NUM 99

typedef struct jugador
{
	int nro;						
	int msqid;
}t_jugador;

void validaParametrosInput(int argc, char *argv[]);
void *ThreadJugador(void *threadParam);


int main(int argc, char *argv[])
{
    validaParametrosInput(argc, argv);
    t_jugador *datos_thread;
    int cantJugadores = atoi(argv[1]);
    int i=0;
    random_init();

    int msqid = createMessageQueue(4532);
    
    pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantJugadores);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
    pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
    
    datos_thread = (t_jugador*) malloc(sizeof(t_jugador)*cantJugadores);
	
	for(i=0; i<cantJugadores; i++)
	{
		datos_thread[i].nro = i;
		datos_thread[i].msqid = msqid;
		
		pthread_create (&idHilo[i], &atributos, ThreadJugador, &datos_thread[i]);
	}
	
	for(i=0; i<cantJugadores; i++)
	{
		pthread_join (idHilo[i], NULL);
    }			
    
    return 0;
}


//Declaracion de funciones

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir el número de jugadores\n\n");
        exit(1);
    }
    
    int val = atoi(argv[1]);

    if (val < 1 || val > CANT_MAX_JUGADORES)
    {
        printf("\nEl nro de jugadores no cumple con los permitidos\n\n");
        exit(1);
    }
}



void *ThreadJugador (void *threadParam)
{
    int cantIntentos = 0;
    t_jugador* j = (t_jugador*)threadParam;
    int nroJugador = j->nro;
    int msqid = j->msqid;
    int done = 0;
    
    MessageQueue mq;
    mq.info.event = EVT_NUMERO;
    mq.info.nro_jugador = nroJugador;
    mq.mtype = MSG_PIENSO;
    printf("\nthread %d", nroJugador);

    while (done == 0)
    {
        MessageQueue rtaMsg;
        rtaMsg.mtype = MSG_JUGADOR + nroJugador;

        mq.info.nro_pensado = random_generate(MIN_NUM, MAX_NUM);

        printf("\nVoy a enviar");
        showMessageQueue(&mq);
        sendMessageQueue(msqid, &mq);

        printf("\nEspero recibir");
        recvMessageQueue(msqid, &rtaMsg);
        printf("recibi\n");
        showMessageQueue(&rtaMsg);

        switch(rtaMsg.info.event)
        {
            case EVT_ACERTO:
                printf("\nAcertaste en %d intentos", cantIntentos+1);
                break;
            case EVT_FINALIZAR:
                done = 1;
                break;
            case EVT_NO_ACERTO:
                printf("\nno acerto");
        }

        cantIntentos++;
        usleep(100 * 1000);
    }
    
    pthread_exit ((void *)"Listo");
}