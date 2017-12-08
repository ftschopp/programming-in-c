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


typedef struct s_artificiero
{
    int msqid;
	int nro;						
    int cable;
    int cantCablesBomba;
    int cantArtificieros;
}t_artificiero;

void validaParametrosInput(int argc, char *argv[]);
void *ThreadArtificiero(void *threadParam);
void finalizarArtificieros(int msqid, int artificieroOrigen, int cantArtificieros);

int main(int argc, char *argv[])
{
    random_init();
    validaParametrosInput(argc, argv);
    int cantCables = atoi(argv[1]);
    int cantArtificieros = 0;
    int msqid = createMessageQueue(ID_MQ);
    int i = 0;

    printf("\nIngrese la cantidad de artificieros:");
    scanf("%d",&cantArtificieros);
    fflush(stdin);

    printf("\n Valor ingresado :%d", cantArtificieros); 

    t_artificiero *datos_thread;
    
    pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantArtificieros);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
    pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
    
    datos_thread = (t_artificiero*) malloc(sizeof(t_artificiero)*cantArtificieros);
	
	for(i=0; i < cantArtificieros; i++)
	{
        datos_thread[i].msqid = msqid;
		datos_thread[i].nro = i+1;		
        datos_thread[i].cable = 0;	
        datos_thread[i].cantCablesBomba = cantCables;	
        datos_thread[i].cantArtificieros = cantArtificieros;	
		pthread_create (&idHilo[i], &atributos, ThreadArtificiero, &datos_thread[i]);
	}

	for(i=0; i < cantArtificieros; i++)
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



void *ThreadArtificiero(void *threadParam)
{
    t_artificiero* artificieroParam = (t_artificiero*)threadParam;
    
    int nroArtificiero = artificieroParam->nro;
    int msqid = artificieroParam->msqid;
    int cantCablesBomba = artificieroParam->cantCablesBomba;
    int cantArtificieros = artificieroParam->cantArtificieros;
    
    random_init();
    printf("\n Artificiero %d", nroArtificiero);
    int done = 0;


    while(done == 0)
    {
        int cableElegido = random_generate(1, cantCablesBomba);

        Message msg;
        msg.to = MSG_BOMBA;
        msg.from = MSG_ARTIFICIEROS + nroArtificiero;
        msg.event = EVT_CABLE;
        msg.info.cable = cableElegido;

        sendMessageQueue(msqid, &msg);
        
        Message rta;

        recvMessageQueue(msqid, MSG_ARTIFICIEROS + nroArtificiero, &rta);

        switch(rta.event)
        {
            case EVT_NINGUNO:
            case EVT_CABLE:
                //no hacen nada estos eventos, los agrego por el warinign.
                break;
            case EVT_EXPLOTO:
            case EVT_DESACTIVADA:
                printf("\n Cantidad de intentos: %d finalizando...", rta.info.intentos);
                finalizarArtificieros(msqid, nroArtificiero, cantArtificieros);
                break;
            case EVT_FINALIZAR:
                done = 1;
                break;
            case EVT_OTRO_INTENTO:
                //no hago nada;
                break;
        }

        usleep(100 * 1000);
    }

    pthread_exit ((void *)"Listo");
}

void finalizarArtificieros(int msqid, int artificieroOrigen, int cantArtificieros)
{
    int i =0;
    Message msg;

    msg.event = EVT_FINALIZAR;
    msg.from = MSG_ARTIFICIEROS + artificieroOrigen;
    
    for(i = 0; i < cantArtificieros; i++)
    {
        msg.to = MSG_ARTIFICIEROS + i+1;

        sendMessageQueue(msqid, &msg);
    }
}