#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "sys_v_semaphore.h"
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define TIEMPO_ESPERA 100
#define LARGO_MENSAJE 50
#define CANT_PANELES 3

typedef struct paneles
{
    int nro_panel;
    char mensaje[LARGO_MENSAJE];
} paneles;

//Prototipo de funciones
void *createShareMemory(key_t id, long bytes);
void validaParametrosInput(int argc, char *argv[]);

//Main function
int main(int argc, char *argv[])
{
    key_t keySem, keyMem;
    int semID, i;
    paneles *paneles;

    random_init();
    keySem = creo_clave(1233);
    keyMem = creo_clave(3312);
    semID = sem_mutex_create(keySem);

    paneles = createShareMemory(keyMem, sizeof(paneles) * CANT_PANELES);

    validaParametrosInput(argc, argv);
    int nroPanel = atoi(argv[1]);
    char msjAnterior[LARGO_MENSAJE];
    strcpy(msjAnterior, "NONE");

    while (1)
    {
        sem_mutex_wait(&semID);

        if (strcmp(msjAnterior, paneles[nroPanel - 1].mensaje) != 0)
        {
            strcpy(msjAnterior, paneles[nroPanel - 1].mensaje);
            system("clear");
            printf("ESTADO TRANSITO: %s\n", msjAnterior);
        }

        sem_mutex_post(&semID);
        usleep(1000 * TIEMPO_ESPERA);
    }
}

//definicion de funciones
void *createShareMemory(key_t id, long bytes)
{
    int shmID = shmget(id, bytes, 0666 | IPC_CREAT);
    void *mem;

    if (shmID == -1)
    {
        printf("No consigo id para memoria compartida \n");
        exit(0);
    }

    mem = shmat(shmID, NULL, 0);

    if (mem == NULL)
    {
        printf("No consigo memoria compartida \n ");
        exit(0);
    }

    return mem;
}

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir el número de panel\n\n");
        exit(1);
    }
    
    int val = atoi(argv[1]);

    if (val < 1 || val > CANT_PANELES)
    {
        printf("\nEl nro de panel no cumple con los permitidos\n\n");
        exit(1);
    }
}