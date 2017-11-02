#include "common.h"
#include <stdlib.h>
#include <sys/time.h>

//Genera un numero aleatorio entre 0 y 65536
void rnd(long *prandom)
{
	*prandom = (25173 * *prandom + 13849) % 65536;
}

//Genera un numero aleatorio entre min y max
void rnd_between(long *prandom,long min, long max)
{
	*prandom = (25173 * *prandom + 13849) % (max + 1 - min) + min;
}

void random_init()
{
	struct timeval time; 
	gettimeofday(&time, NULL);
	// microsecond has 1 000 000
	// Assuming you did not need quite that accuracy
	// Also do not assume the system clock has that accuracy.
	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int random_generate(int min, int max)
{
	int number, input;		
		
	/* Generate a random number: */		
	 number = rand() % (max + 1 - min) + min;;
	return number;
}

//Generador de clave a partir de un entero.
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

//genera un segmento de memoria
// int createSegmentMemory(long size)
// {
// 	int idMemorySegment;

// 	if((idMemorySegment = shmget(IPC_PRIVATE, size, 0666)) < 0)
// 	{
// 		perror("shmget:");
// 		exit(EXIT_FAILURE);
// 	}
	
// 	return idMemorySegment;
// }

// void * assignSegementMemory(int idMemorySegment)
// {
// 	void* pointer = shmat(idMemorySegment, )

// }

// void removeSegmentMemory()
// {

// }