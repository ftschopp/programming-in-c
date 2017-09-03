#include "semaphore_posix.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

void assignSemInitializeError()
{
    switch(errno)
    {
        case EACCES:
                errno = EPERM;
                break;
        case EINVAL:
                break;
        case EPERM:
                break;
        case ERANGE:
                errno = EINVAL;
                break;
    }
}
void assignSemDestroyError()
{
    switch(errno)
    {
        case EACCES:
                errno = EINVAL;
                break;
        case EINVAL:
                break;
        case EPERM:
                errno = EINVAL;
                break;
        case ERANGE:
                break;
    }
}
void assignSemgetError()
{
    switch(errno)
    {
        case EACCES:
                errno = EPERM;
                break;
/*      case EEXIT:
                break;  */
        case EINVAL:
                break;
        case ENOENT:
                errno = EINVAL;
                break;
        case ENOSPC:
                break;
        default:
                break;
    }
}
void assignSemopErrorCode()
{
    switch(errno)
    {
        case EACCES:
            errno = EINVAL;
            break;
/*      case EDAMAGE:
            errno = EINVAL;
            break;*/
        case EINVAL:
            break;
        case EFAULT:
            errno = EINVAL;
            break;
        case EFBIG || EIDRM:
            errno = EINVAL;
            break;
/*      case EUNKNOWN:
            printf("\nEUNKNOWN");
            break;*/
        case ERANGE:
            break;
        case ENOSPC:
            break;
        case EINTR:
            break;
        /*case EPERM:
            break;*/
        default:
            errno = EINVAL;
            break;
    }
}

/* initsem -- called by sem_create, it gets the semaphore using semget() */
int initsem(key_t key, int nsems)
{
   int semid;

    semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);
     if (semid == -1 && errno == EEXIST) { /* someone else got it first */
         semid = semget(key, nsems, 0); /* get the id */
         printf("\n Some one else got it ");
         if (semid < 0)
         {
                 return semid; /* error, errno is assigned in the calling fn*/
         }

    } else if(semid == -1) { /*chk for other errors here */
        /*err num is assigned in the calling fn*/
        return semid;
    }

    return semid;
}


/* sem_initialize -- it assigns a value to the semaphore using semctl() */
int sem_initialize(int *semid, int value)
{
 int ret = semctl(*semid,0,SETVAL,value);
 /*if(ret == -1){
    printf("\nSemictrl error in sem_initialise =");
    perror(errno);
 }*/
 return ret;
}

/* sem_init --
 * it accepts key and semaphore value as its parameters
 * it creates a semaphore using semget and then initialize it
 * it returns semid
*/
int sem_init(int *sem,int pshared,unsigned int value)
{
  int ret = -1;
  key_t key = 5600;
  time_t seconds;
  seconds = time(NULL);
  if(value > SEM_VALUE_MAX){
     errno = EINVAL;
     return ret;
   }
  key = seconds;
/*  printf("\ntime = %ld, key = %ld",seconds,key);*/
  if((*sem = initsem(key,1)) == -1){
       /*assign err code*/
       assignSemgetError();
       return -1;
  }
  else
     ret = sem_initialize(sem,value);
  if(ret == -1)
  {
    /* assign errcode for semctl */
     assignSemInitializeError();
  }
  return ret;
}

/* sem_destroy -- destroys the semaphore using semctl() */
int sem_destroy(int *semid)
{
  int ret = semctl(*semid,0,IPC_RMID);
  if(ret == -1)
  {
 /* assign err code for semctl*/
    assignSemDestroyError();
   }
  return ret;
}

/* sem_wait -- it gets a lock on semaphore and implemented using semop() */
int sem_wait(int *semid)
{
        struct sembuf sb;
        sb.sem_num=0;
        sb.sem_op=-1;
        sb.sem_flg=0;
        if (semop(*semid, &sb, 1) == -1) {
           /*printf("\nSemiop Error in sem_wait=");*/
           assignSemopErrorCode();
           /*printf("\nAfter assigning error in semwait:");
           perror(errno);*/
           return -1;
        }
        return 0;
}

/* sem_trywait --
 * this function will lock the semaphore if semaphore is currently available to be locked otherwise it will not lock semaphore.
 * implemented using semctl() and semop()
 * if part - checks the value of semaphore to check whether its available. If returns true - it comes out as it cannot be locked.
 * else part - if semaphore is available to be locked, it gets a lock on semaphore.
 */
int sem_trywait(int *semid)
{
        struct sembuf sb;
        sb.sem_num=0;
        sb.sem_op=-1;
        sb.sem_flg=IPC_NOWAIT;
        if (semop(*semid, &sb, 1) == -1) {
          /* printf("\nSemop error in sem_trywait");
           perror(errno);*/
           assignSemopErrorCode();
           /*printf("\nAfter assigning error in sem_trywait:");
           perror(errno);*/
           return -1;
        }
        return 0;
}

/* sem_post -- it releases lock on semaphore using semop */
int sem_post(int *semid)
{
        struct sembuf sb;
        sb.sem_num=0;
        sb.sem_op=1;
        sb.sem_flg=0;
        if (semop(*semid, &sb, 1) == -1) {
            /* perror(errno);*/
             assignSemopErrorCode();
           /*  printf("\nAfter assigning error in sem_post:");
             perror(errno);*/
             return -1;
        }
        return 0;
}
