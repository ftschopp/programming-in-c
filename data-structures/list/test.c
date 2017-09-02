#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
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

    insert(&first, &last, info1);
    insert(&first, &last, info2);
    insert(&first, &last, info3);
    printf("Mostramos en orden normal");
    show_list(&first);

    printf("Mostramos en orden inverso");
    show_list_reverse(&last);
    
    printf("borro");
    delete_by_key(&first, &last, 2);
    show_list(&first);

    printf("borro");
    delete_by_key(&first, &last, 1);
    show_list(&first);

    printf("borro");
    delete_by_key(&first, &last, 3);
    show_list(&first);
    return 0;

}