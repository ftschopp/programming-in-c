#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>

// union semun {
// int val;
// struct semid_ds *buf;
// unsigned short *array;
// struct seminfo *__buf;
// };

int obtenerMutex(key_t clave);
int obtenerSemaforo(key_t clave, int valor);
void pedirSemaforo(int IdSemaforo);
void devolverSemaforo(int IdSemaforo);
void eliminarSemaforo(int IdSemaforo);
void eliminarMutex(int IdSemaforo);
