#include <stdio.h>
#include <stdlib.h>
#include "common.h"

// Driver program to test above function.
int main()
{
    int cantidad = 15;
    int startNumber = 0;
    
    int *vector =(int*) malloc(sizeof(int)*cantidad);
    
    int *pri = vector;

    for(int i=0; i < cantidad; i++)
    {
        *pri = ++(startNumber);
        pri++;
    }

     pri = vector;
     shuffleArray(pri, cantidad);
     pri = vector;
     printArray(pri, cantidad);
 
    return 0;
}