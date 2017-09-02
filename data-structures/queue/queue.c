#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"


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
void push(node **first, node **last, data info)
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

//pop obtiene el primer elemento de la cola
node* pop(node **first) 
{
    node *aux = NULL;
   if(!isEmpty(*first))
   {
        aux = (*first);        
        (*first) = (*first)->next;
   }
   
    return aux;
}