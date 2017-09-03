#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include "sys_v_semaphore.h"

#define LARGO_MAX 100
#define CANTIDAD 20
#define INTERVALO 1

key_t creo_clave(int id)
{
  key_t clave = ftok("/bin/ls", id); 
  
  if(clave == (key_t)-1)
  {
      printf("Error no puedo conseguir clave\n");
      exit(0);
  }
  
  return clave;
}

int main()
{
	char cadena[LARGO_MAX];
	FILE *productor;
	int i;
    int partida = 0;
    
    key_t key = creo_clave(55);

	int semaforoid = sem_mutex_create(key);
	
	sem_mutex_init(&semaforoid, 1);
	
	while(1)
	{

        printf("\n voy a esperar...");
		sem_mutex_wait(&semaforoid);
        
        // printf("\n entre en seccion critica");
        // getchar();
		productor = fopen("p.txt","w");

		if(productor == NULL)
		{
			perror("Error p.txt");
			return -1;
		}
		
		for(i=0;i<=CANTIDAD;i++)
		{
			sprintf(cadena,"producto -%02d-%02d\n",partida,i);
			printf("%s",cadena);
			fputs(cadena,productor);
			sleep(INTERVALO);
			partida++;
		}
		fclose(productor);

		sem_mutex_post(&semaforoid);
	
		sleep(INTERVALO);
	};

	return 0;
}