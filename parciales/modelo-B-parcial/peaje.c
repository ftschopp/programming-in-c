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

#define LIMITE_AUTOS_POR_VIA 10
#define TIEMPO_ESPERA 1000 * 1500

//Declaracion de funciones
void validaParametrosInput(int argc, char *argv[]);
void informarViaLlena(int cantidad);
void mostrarVias(int *vias, int cantVias);
void *createShareMemory(key_t id, long bytes);
void destroyShareMemory(key_t id);
void inicializarMemoria(int *p, int cantVias);

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
    inicializarMemoria(vias, cantVias);
    sem_mutex_init(&semID, 1);

    while (1)
    {
        int j = 0;
        int msToWait = random_generate(100, 5000);

        for (j = 0; j < cantVias; j++)
        {
            sem_mutex_wait(&semID);

            if (vias[j] > LIMITE_AUTOS_POR_VIA)
            {
                vias[j] = 0;
                cantidadLiberaciones++;
                informarViaLlena(cantidadLiberaciones);
            }
            else if (vias[j] > 0)
            {
                vias[j]--;
            }

            
            mostrarVias(vias, cantVias);
            
            sem_mutex_post(&semID);

            usleep(1000 * msToWait);
        }
    }
}

//Definicion de funciones
void informarViaLlena(int cantidad)
{
    int num;
    FILE *fptr;
    fptr = fopen("liberaciones.txt", "w");

    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "%d", cantidad);
    fclose(fptr);
}

void mostrarVias(int *vias, int cantVias)
{
    system("clear");
    //printf("\n\n----------------\n");
    int i;

    for (i = 0; i < cantVias; i++)
    {
        printf("VIA[%d] %d Vehiculos\n", i + 1, vias[i]);
    }
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

void destroyShareMemory(key_t id)
{
    shmctl(id, IPC_RMID, NULL);
}

void inicializarMemoria(int *p, int cantVias)
{
    int i = 0;

    for (i = 0; i < cantVias; i++)
    {
        p[i] = 0;
    }
}
