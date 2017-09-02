#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"


//Definicion de funciones

//Creacion de nuevo nodo en memoria
node* new_node()
{
    node *q;
    
    q = (node*)malloc(sizeof(struct node));
    
    if ( q == NULL )
 	{
        printf("MEMORIA INSUFICIENTE!!!");
        exit(2);
    }

    return q;
}

int isEmpty(node* t)
{
   return t == NULL;
}

//inserta al final
void insert(node **first, node **last, data info)
{
    node *q;
    q = new_node();
    q->info = info;
    q->next = NULL;

    if(isEmpty((*first)))
 	{
        (*first) = q;
        (*last) = q;
    }
    else
   	{
        (*last)->next = q;
        (*last) = q;
    }
}


//remove
data remove(node **first) 
{
   data info = NULL;
   
   if(!isEmpty(*first))
   {
        info = (*first)->info;
        (*first) = (*first)->next;
        free()
   }
   
    return data;
}