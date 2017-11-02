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

#define CANT_JUGADORES 2
#define CANT_TIRADAS 3
#define TIEMPO_ESPERA 1000 * 1500
#define EMPATE 3
#define GANOJ1 1
#define GANOJ2 2
#define PIEDRA 1
#define PAPEL 2
#define TIJERA 3

typedef struct paneles
{
    int estado;
    int jugador[CANT_JUGADORES];
} paneles;

//Declaracion de funciones
void validaParametrosInput(int argc, char *argv[]);
void setearJugadores(char *argv[], int *jugadorActual, int *jugadorOponente);
//Incializar memoria
void inicializarMemoria(paneles *p);
void mostrarDatos(paneles *tirada);

//Share memory
void *createShareMemory(key_t id, long bytes);
void destroyShareMemory(key_t id);

//mostrar datos
void mostrarDatosPartida(paneles *tiradas, int partidaActual);
int determinarResultado(paneles *tiradas, int partidaActual);
//void determinarResultado(paneles *tiradas, int partidaActual);

//Main function
int main(int argc, char *argv[])
{
    int shmID, i;
    paneles *tiradas, *pointer;
    int jugadorActual, jugadorOponente;
    int termineJuego = 0, partidaActual = 0;
    key_t keySem, keyMem;
    int semID;
    int resultadoPartida[2] = {0, 0};
    int partidaTerminada = 0;

    validaParametrosInput(argc, argv);
    random_init();
    keySem = creo_clave(1233);
    keyMem = creo_clave(3312);
    semID = sem_mutex_create(keySem);

    tiradas = createShareMemory(keyMem, sizeof(paneles) * CANT_TIRADAS);

    setearJugadores(argv, &jugadorActual, &jugadorOponente);

    if (jugadorActual == 0)
    {
        inicializarMemoria(tiradas);
        //mostrarDatos(tiradas);
        sem_mutex_init(&semID, 1);
    }

    long valorJugado = 0;
    int jugaronAmbos = 0;
    while (partidaActual < CANT_TIRADAS)
    {
        sem_mutex_wait(&semID);
        if (tiradas[partidaActual].jugador[jugadorActual] == 0)
        {
            //no jugo, por lo tanto debe elegir piedra papel o tijera
            tiradas[partidaActual].jugador[jugadorActual] = random_generate(1, 3);
        }
        else if (tiradas[partidaActual].jugador[jugadorOponente] != 0)
        {
            //en este caso yo juge y el oponente tambien con lo cual debo ver el resultado de la partida
            int res = determinarResultado(tiradas, partidaActual);

            if (res == GANOJ1)
            {
                resultadoPartida[jugadorActual]++;
            }
            else if (res == GANOJ2)
            {
                resultadoPartida[jugadorOponente]++;
            }
            jugaronAmbos = 1;
            //printf("\n %d %d", resultadoPartida[0],  resultadoPartida[1]);

            //printf("\n J1 :%d J2: %d  EST %d", tiradas[partidaActual].jugador[0],tiradas[partidaActual].jugador[1], tiradas[partidaActual].estado);
            //mostrarDatosPartida(tiradas, partidaActual);
            //printf("\n----------------------------------------");
            // partidaActual++;
        }

        mostrarDatosPartida(tiradas, partidaActual);
        if (jugaronAmbos)
        {
            partidaActual++;
            jugaronAmbos = 0;
        }

        sem_mutex_post(&semID);

        // printf("\n Mi turno %d", tiradas[1].estado);
        usleep(TIEMPO_ESPERA);
    }

    printf("\nResultado Global %d %d ", resultadoPartida[jugadorActual], resultadoPartida[jugadorOponente]);
    if (resultadoPartida[jugadorActual] > resultadoPartida[jugadorOponente])
    {
        printf("\n GANO J1");
    }
    else if (resultadoPartida[jugadorActual] < resultadoPartida[jugadorOponente])
    {
        printf("\n GANO J2");
    }
    else
    {
        printf("\nHUBO EMPATE");
    }
}

//Definicion de funciones

void mostrarDatosPartida(paneles *tiradas, int partidaActual)
{
    int i = 0, j;
    char op1[30], op2[30], res[30];

    switch (tiradas[partidaActual].jugador[0])
    {
    case PIEDRA:
        strcpy(op1, "PIEDRA");
        break;
    case PAPEL:
        strcpy(op1, "PAPEL");
        break;
    case TIJERA:
        strcpy(op1, "TIJERA");
        break;
    default:
        strcpy(op1, "NO JUGO");
        break;
    }

    switch (tiradas[partidaActual].jugador[1])
    {
    case PIEDRA:
        strcpy(op2, "PIEDRA");
        break;
    case PAPEL:
        strcpy(op2, "PAPEL");
        break;
    case TIJERA:
        strcpy(op2, "TIJERA");
        break;
    default:
        strcpy(op2, "NO JUGO");
        break;
    }

    switch (tiradas[partidaActual].estado)
    {
    case EMPATE:
        strcpy(res, "EMPATE");
        break;
    case GANOJ1:
        strcpy(res, "GANO J1");
        break;
    case GANOJ2:
        strcpy(res, "GANO J2");
        break;
    default:
        strcpy(res, "SIN DETERMINAR");
        break;
    }

    printf("\n#%d  %s VS %s  resultado: %s", partidaActual + 1, op1, op2, res);
}

int determinarResultado(paneles *tiradas, int partidaActual)
{
    if (tiradas[partidaActual].jugador[0] == tiradas[partidaActual].jugador[1])
    {
        //empate
        tiradas[partidaActual].estado = EMPATE;
    }
    else if (tiradas[partidaActual].jugador[0] == PIEDRA && tiradas[partidaActual].jugador[1] == TIJERA)
    {
        tiradas[partidaActual].estado = GANOJ1;
    }
    else if (tiradas[partidaActual].jugador[0] == PAPEL && tiradas[partidaActual].jugador[1] == PIEDRA)
    {
        tiradas[partidaActual].estado = GANOJ1;
    }
    else if (tiradas[partidaActual].jugador[0] == TIJERA && tiradas[partidaActual].jugador[1] == PAPEL)
    {
        tiradas[partidaActual].estado = GANOJ1;
    }
    else
    {
        tiradas[partidaActual].estado = GANOJ2;
    }

    return tiradas[partidaActual].estado;
}

void validaParametrosInput(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nDebe recibir el número de jugador\n\n");
        exit(1);
    }
}
//funcion que setea los id de jugadores en el proceso
void setearJugadores(char *argv[], int *jugadorActual, int *jugadorOponente)
{
    int nro = atoi(argv[1]);
    if (nro == 1)
    {
        *jugadorActual = 0;
        *jugadorOponente = 1;
    }
    else
    {
        *jugadorActual = 1;
        *jugadorOponente = 0;
    }
}

//Inicializa la memoria
void inicializarMemoria(paneles *tirada)
{
    int i = 0;

    for (i = 0; i < CANT_TIRADAS; i++)
    {
        tirada[i].estado = 0;
        tirada[i].jugador[0] = 0;
        tirada[i].jugador[1] = 0;
    }
}

void mostrarDatos(paneles *tirada)
{
    paneles *p = tirada;
    int i = 0;

    for (i = 0; i < CANT_TIRADAS; i++)
    {
        printf("\n Estado %d  J1 %d  J2 %d", p->estado, p->jugador[0], p->jugador[1]);
        p++;
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