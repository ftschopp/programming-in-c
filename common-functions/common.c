#include "common.h"
#include <stdlib.h>

//Genera un numero aleatorio entre 0 y 65536
void rnd(long *prandom)
{
	*prandom = (25173 * *prandom + 13849) % 65536;
}

// C Program to shuffle a given array
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// A utility function to print an array
void printArray (int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
		printf("%d ", *arr);
		arr++;
	}   
    printf("\n");
}

void shuffleArray(int *array, size_t n) {    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int usec = tv.tv_usec;
    srand48(usec);


    if (n > 1) {
        size_t i;
        for (i = n - 1; i > 0; i--) {
            size_t j = (unsigned int) (drand48()*(i+1));
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
