#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <sys/sem.h>
#include "semaphore_posix.h"

int count = 0;
int sem_id;

void * ThreadFunction1(void * a)
{
	pthread_t threadid;
	threadid = pthread_self();
        int ret = sem_trywait(&sem_id);
        if(ret == 0)
        {
 	   printf("\n Threadid  id =%d(ThreadFunction1), locked and incrementing the count ",threadid);
           count = count + 1;
	   printf("\tCount (ThreadFunction1) = %d",count);
           sleep(10);
           sem_post(&sem_id);
	   printf("\nThread %d going to release lock ",threadid);
        }
        else {
	   printf("\n Thread id %d is in ThreadFunction1, semaphore is already locked and not waiting. Hence will not increment the counter",threadid);
	}
}

void *ThreadFunction2( void *ptr )
{
     char *message;
     pthread_t threadid;
     int ret = sem_wait(&sem_id);
     threadid = pthread_self();
     if (ret == 0) {
	printf("\n Thread id = %d(ThreadFunction2) , locked and incrementing the count",threadid);
     count = count +1;
     printf("\tCount (ThreadFunction2) = %d",count);
     sleep(10);
     sem_post(&sem_id);
     printf("\nThread id %d going to release lock ",threadid);
     }
}


main()
{
        pthread_t thread[10];
        int  iret1, j;
	char counter[20];
        int ret;

        sem_init(&sem_id,0, 1);

        for(j=0;j<10;j++)
        {
		if ( (j%3 == 0) || (j == 0) ) {
		iret1 = pthread_create(&thread[j],NULL,ThreadFunction1 ,(void*)counter );
	        printf("\nThread no %d id = %d created",j,thread[j]);
		}
		else {
                iret1 = pthread_create(&thread[j],NULL,ThreadFunction2,(void*)counter );
		printf("\nThread no %d id = %d created",j,thread[j]);
		}
		
        }
        for(j=0;j<10;j++)
        {
                if(pthread_join(thread[j], NULL))       /* wait for others thread to finish */
                {
                        printf("\n ERROR joining thread");
                        exit(1);
                }
        }
        printf("\nAll threads joined, Final count = %d\n",count);
        sem_destroy(&sem_id);
	ret = sem_post(&sem_id);
}

