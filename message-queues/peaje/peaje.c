#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "sys_v_semaphore.h"
#include "common.h"
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

//declaracion de funciones
void validaParametrosInput(int argc, char *argv[], int *cantVias);
void createThreads( pthread_t **thread_id, int count);
void waitThreads(pthread_t *thread_id, int count);
void *thread_function(void *dummyPtr);


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int semID;
//Main function
int main(int argc, char *argv[])
{
    key_t keySem;
    // int semID;
    
    pthread_t *thread_id;

    int cantVias = 0;
    validaParametrosInput(argc, argv, &cantVias);
    
    keySem = creo_clave(1333);
    semID = sem_create(keySem, cantVias);
    
    for(int i=0; i < cantVias; i++)
    {
        printf("Entro i \n");
        sem_wait(&semID,0);
    }

    createThreads(&thread_id, cantVias);
    
    for(int i=0; i< cantVias; i++)
    {
        sem_post(&semID,0);
    }

    waitThreads(&thread_id, cantVias);


    printf("Fin JOIN\n");
    for (int i = 0; i < cantVias; i++)
    {
        printf("Hola %d\n", i);
    }

    return 0;
}

//definiciones de funciones
void validaParametrosInput(int argc, char *argv[], int *cantVias)
{
    if (argc != 2)
    {
        printf("\nDebe recibir el número de vias de peaje\n\n");
        exit(1);
    }

    int val = atoi(argv[1]);
    *cantVias = val;

    if (val < 0)
    {
        printf("\nEl nro de vias de peaje no cumple con los permitidos\n\n");
        exit(1);
    }
}

void createThreads(pthread_t **thread_id, int count)
{
    (*thread_id) = malloc(sizeof(pthread_t)*count);
   
    for(int i = 0; i < count; i++)
    {
        pthread_create( &(*thread_id)[i], NULL, thread_function, NULL );
    }
    printf("Threads Creados\n");
}

void waitThreads(pthread_t *thread_id, int count)
{
    for(int j=0; j < count; j++)
    {
       pthread_join(&thread_id[j], NULL);
    }
}

void *thread_function(void *dummyPtr)
{
   //sem_wait(semID,i);
   printf("Ejecuto thread number %ld\n", pthread_self());
   pthread_mutex_lock( &mutex1 );
   //counter++;
   pthread_mutex_unlock( &mutex1 );
}