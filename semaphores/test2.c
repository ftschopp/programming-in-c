#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "sys_v_semaphore.h"
#define INTERVALOMICROSEGUNDO 500000 //medio segundo
#define LARGO_MAX 100

key_t creo_clave(int id)
{
	key_t clave = ftok("/bin/ls", id);

	if (clave == (key_t)-1)
	{
		printf("Error no puedo conseguir clave\n");
		exit(0);
	}

	return clave;
}

int main()
{
	printf("Consumimos \n");

	FILE *consumidor;
	char cadena[LARGO_MAX];

	key_t key = creo_clave(55);
	int semaforoid = sem_mutex_create(key);

	while (1)
	{
		printf("\n voy a esperar...");
		sem_mutex_wait(&semaforoid);

		printf("\n entre en seccion critica");
		consumidor = fopen("p.txt", "r");

		printf("abriendo archivo \n");
		if (consumidor == NULL)
		{
			perror("Error p.txt");
			return -1;
		}
		while (fgets(cadena, LARGO_MAX, consumidor) != NULL)
		{
			printf("%s", cadena);
			usleep(INTERVALOMICROSEGUNDO);
		};

		fclose(consumidor);
		remove("p.txt");

		sem_mutex_post(&semaforoid);

		usleep(INTERVALOMICROSEGUNDO);
	};

	return 0;
}