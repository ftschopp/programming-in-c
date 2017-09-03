/*
 *ftschopp 
 * semaphores system V
 * 
 */

#ifndef SYS_V_SEMAPHORE_H
#define SYS_V_SEMAPHORE_H

#include <sys/sem.h>

//Crea un conjunto de semaforos
int sem_create(key_t key, int count);

//Crea un semaforo mutex
int sem_mutex_create(key_t key);

//inicializa un determinado semaforo
void sem_init(int *semid, int nsem, int value);

//inicializa un semaforo mutex
void sem_mutex_init(int *semid, int value);

// decrementa el semaforo
void sem_wait(int *semid, int nsem);

// incrementa el semaforo
void sem_post(int *semid, int nsem);

// decrementa el semaforo
void sem_mutex_wait(int *semid);

// incrementa el semaforo
void sem_mutex_post(int *semid);

//elimina el semaforo de los recursos del SO
void sem_destroy(int *semid);


#endif
