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
//Declaracion de funciones
void validaParametrosInput(int argc, char *argv[]);
int buscoViaConMenorAuto(int *p, int cantVias);
void *createShareMemory(key_t id, long bytes);

//Main function
int main(int argc, char *argv[])
{
    key_t keySem, keyMem;
    int semID;
    int *vias;
    validaParametrosInput(argc, argv);
    int cantVias = atoi(argv[1]);
    int cantidadLiberaciones = 0;

    random_init();
    keySem = creo_clave(755);
    keyMem = creo_clave(655);

    semID = sem_mutex_create(keySem);
    vias = createShareMemory(keyMem, sizeof(int) * cantVias);

    while (1)
    {
        sem_mutex_wait(&semID);
        int msToWait = random_generate(400, 600);
        int menorVia = buscoViaConMenorAuto(vias, cantVias);

        printf("\nAuto Ingresando a VIA %d", menorVia + 1);
        vias[menorVia]++;

        sem_mutex_post(&semID);
        usleep(1000 * msToWait);
    }
}

int buscoViaConMenorAuto(int *p, int cantVias)
{
    int j = 0;
    int min = p[0];
    int idx = 0;

    for (j = 0; j < cantVias; j++)
    {
        if (p[j] < min)
        {
            min = p[j];
            idx = j;
        }
    }

    return idx;
}

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir el número de jugador\n\n");
        exit(1);
    }
}

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