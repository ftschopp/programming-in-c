#include "semaforos.h"

int obtenerMutex(key_t clave)
{
	int IdSemaforo;
	union semun CtlSem;
	IdSemaforo = semget(clave, 1, IPC_CREAT | 0600);
	CtlSem.val = 1;
	semctl(IdSemaforo, 0, SETVAL, CtlSem);
	return IdSemaforo;
}

int obtenerSemaforo(key_t clave, int valor) 
{
	int IdSemaforo;
	union semun CtlSem;
	IdSemaforo = semget(clave, 1, IPC_CREAT | 0600);
	CtlSem.val = valor;
	semctl(IdSemaforo, 0, SETVAL, CtlSem);
	return IdSemaforo;
}

void pedirSemaforo(int IdSemaforo) 
{
	struct sembuf OpSem;
	OpSem.sem_num = 0;
	OpSem.sem_op = -1;
	OpSem.sem_flg = 0;
	semop(IdSemaforo, &OpSem, 1);
}

void devolverSemaforo(int IdSemaforo)
{
	struct sembuf OpSem;
	OpSem.sem_num = 0;
	OpSem.sem_op = 1;
	OpSem.sem_flg = 0;
	semop(IdSemaforo, &OpSem, 1);
}

void eliminarSemaforo(int IdSemaforo)
{
	semctl(IdSemaforo, 0, IPC_RMID);
}

void eliminarMutex(int IdSemaforo) 
{
	semctl(IdSemaforo, 0, IPC_RMID);
}
