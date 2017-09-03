#include <sys/sem.h>
#include <errno.h>


int initsem(key_t key, int nsems);

int sem_create(key_t key);

int sem_initialize(int *semid, int value);

int sem_init(int *semid,int pshared,unsigned int value);

int sem_destroy(int *semid);

int sem_wait(int *semid);

int sem_trywait(int *semid);

int sem_post(int *semid);

