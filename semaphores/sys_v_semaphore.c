#include "sys_v_semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>


//Crea un conjunto de semaforos
int sem_create(key_t key, int count)
{
    int idsem;
	idsem = semget(key, count, IPC_CREAT | 0600);
    
    if(idsem == -1)
	{
		perror("sem_create:");
		exit(-1);
    }
    
	return idsem;
}

//Crea un solo semaforo
int sem_mutex_create(key_t key)
{
    return sem_create(key, 1);
}

//inicializa un determinado semaforo
void sem_init(int *semid, int nsem, int value)
{
    if(semctl(*semid, nsem, SETVAL, value) == -1)
    {
        perror("sem_init:");
        exit(-2);
    }
}

//inicializa el primer semaforo del grupo
void sem_mutex_init(int *semid, int value)
{
    sem_init(semid, 0, value);
}

void sem_wait(int *semid, int nsem)
{
    struct sembuf OpSem;
	OpSem.sem_num = nsem;
	OpSem.sem_op = -1;
    OpSem.sem_flg = 0;
    
    if(semop(*semid, &OpSem, 1) == -1)
    {
        perror("sem_wait:");
        exit(-2);
    }
}

void sem_post(int *semid, int nsem)
{
    struct sembuf OpSem;
	OpSem.sem_num = nsem;
	OpSem.sem_op = 1;
	OpSem.sem_flg = 0;

    if(semop(*semid, &OpSem, 1) == -1)
    {
        perror("sem_post:");
        exit(-2);
    }
}

void sem_mutex_wait(int *semid)
{
   sem_wait(semid, 0);
}

void sem_mutex_post(int *semid)
{
    sem_post(semid, 0);
}

void sem_destroy(int *semid)
{
    if(semctl(*semid, 0, IPC_RMID) == -1)
    {
        perror("sem_destroy:");
        exit(-2);
    }
}
