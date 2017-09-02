#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <string.h>



int main(int argc, const char * argv[]) 
{
    node *first = NULL;
    node *last = NULL;

    data info1;
    info1.key = 1;
    strncpy(info1.name, "fernando", 30);

    data info2;
    info2.key = 2;
    strncpy(info2.name, "juan", 30);
    
    data info3;
    info3.key = 3;
    strncpy(info3.name, "pedro", 30);

    push(&first, &last, info1);
    push(&first, &last, info2);
    push(&first, &last, info3);
    
    node* element = pop(&first);

    printf("\nKey Nodo: %d", element->info.key);
    printf("\nName: %s", element->info.name);

    free(element);

    element = pop(&first);
    
    printf("\nKey Nodo: %d", element->info.key);
    printf("\nName: %s", element->info.name);
    
    free(element);

    return 0;

}