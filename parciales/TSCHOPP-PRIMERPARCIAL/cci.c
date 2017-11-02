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

#define TIME_MIN_MS 100
#define TIME_MAX_MS 500
#define LARGO_MENSAJE 50
#define CANT_PANELES 3
#define TRANSITO_NORMAL 1
#define TRANSITO_LENTO 2
#define TRANSITO_DEMORAS 3
#define TRANSITO_CONGESTION 4

typedef struct paneles
{
    int nro_panel;
    char mensaje[LARGO_MENSAJE];
} paneles;

//Prototipo de funciones
void *createShareMemory(key_t id, long bytes);
void inicializarMemoria(paneles *paneles);
void mostrarDatosPaneles(paneles *paneles);

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
    inicializarMemoria(paneles);

    random_init();
    keySem = creo_clave(755);
    keyMem = creo_clave(655);

    sem_mutex_init(&semID, 1);

    while (1)
    {
        int tiempoEspera = random_generate(TIME_MIN_MS, TIME_MAX_MS);
        int nroPanelRandom = random_generate(1, CANT_PANELES);
        int msjEstado = random_generate(TRANSITO_NORMAL, TRANSITO_CONGESTION);

        sem_mutex_wait(&semID);

        switch (msjEstado)
        {
            case TRANSITO_NORMAL:
                strcpy(paneles[nroPanelRandom - 1].mensaje, "NORMAL");
                break;
            case TRANSITO_LENTO:
                strcpy(paneles[nroPanelRandom - 1].mensaje, "LENTO");
                break;
            case TRANSITO_DEMORAS:
                strcpy(paneles[nroPanelRandom - 1].mensaje, "DEMORAS");
                break;
            case TRANSITO_CONGESTION:
                strcpy(paneles[nroPanelRandom - 1].mensaje, "CONGESTION");
                break;
            default:
                strcpy(paneles[nroPanelRandom - 1].mensaje, "NORMAL");
                break;
        }

        sem_mutex_post(&semID);

        //mostrarDatosPaneles(paneles);
        //getchar();
        usleep(tiempoEspera * 1000);
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

//Inicializa la memoria
void inicializarMemoria(paneles *paneles)
{
    int i;
    for (i = 0; i < CANT_PANELES; i++)
    {
        paneles[i].nro_panel = i + 1;
        strcpy(paneles[i].mensaje, "NORMAL");
    }
}

void mostrarDatosPaneles(paneles *mem)
{
    paneles *p = mem;
    int i = 0;

    for (i = 0; i < CANT_PANELES; i++)
    {
        printf("Nro Panel %d Estado %s \n", p->nro_panel, p->mensaje);
        p++;
    }
}
