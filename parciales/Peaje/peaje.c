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
#define MIN_N 1
#define MAX_N 4

typedef struct via
{
	int nro;						
	int msqid;
    int cantidadLiberaciones;
}t_via;

void validaParametrosInput(int argc, char *argv[]);
void *ThreadVia(void *threadParam);


int main(int argc, char *argv[])
{
    validaParametrosInput(argc, argv);

    t_via *datos_thread;
    int cantVias = atoi(argv[1]);
    int i=0;
    random_init();

    int msqid = createMessageQueue(2487);
    
    pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantVias);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
    pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
    
    datos_thread = (t_via*) malloc(sizeof(t_via)*cantVias);
	
	for(i=0; i <cantVias; i++)
	{
		datos_thread[i].nro = i+1;
		datos_thread[i].msqid = msqid;
        datos_thread[i].cantidadLiberaciones = 0;
		
		pthread_create (&idHilo[i], &atributos, ThreadVia, &datos_thread[i]);
	}

	int totalLiberaciones = 0;
	for(i=0; i < cantVias; i++)
	{
		pthread_join (idHilo[i], NULL);
        printf("\nRecibi de la via %d %d liberaciones",i+1, datos_thread[i].cantidadLiberaciones);
        totalLiberaciones+= datos_thread[i].cantidadLiberaciones;
    }			
    		
	printf("\nTOTAL %d\n", totalLiberaciones);
    return 0;
}


//Declaracion de funciones

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir el número de vias\n\n");
        exit(1);
    }
    
    int val = atoi(argv[1]);

    if (val < 1 || val > CANT_MAX_VIAS)
    {
        printf("\nEl nro de vias no cumple con los permitidos\n\n");
        exit(1);
    }
}



void *ThreadVia(void *threadParam)
{
    int cantAutosEnCola = 0;
    int cantLiberaciones = 0;
    t_via* viaParam = (t_via*)threadParam;
    
    int nroVia = viaParam->nro;
    int msqid = viaParam->msqid;
    
    int done = 0;
    random_init();
    printf("\nthread %d", nroVia);

    while (done == 0)
    {
        Message msg;

        printf("\nSoy Via %d y Tengo %d Autos", nroVia, cantAutosEnCola);
        recvMessageQueue(msqid, MSG_VIAS + nroVia, &msg);

        switch(msg.event)
        {
            case EVT_NINGUNO:
                break;
            case EVT_VEHICULOS:
                cantAutosEnCola += msg.info.cantidad;
                int descuenta = random_generate(MIN_N, MAX_N);

                if(cantAutosEnCola > 10)
                {
                    printf("\n Abrir Barreras!!");
                    cantAutosEnCola = 0;
                    cantLiberaciones++;
                }
                else
                {
                    if(cantAutosEnCola - descuenta > 0)
                    {
                        cantAutosEnCola = cantAutosEnCola - descuenta;
                    }
                    else
                    {
                        cantAutosEnCola = 0;
                    }
                }
                
                break;
            case EVT_FINALIZAR:
                done = 1;
                break;
        }
        usleep(100 * 1000);
    }
    
    viaParam->cantidadLiberaciones = cantLiberaciones;
    pthread_exit ((void *)"Listo");
}