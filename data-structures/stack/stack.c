#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"


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

//inserta al principio de la pila
void push(node **first, data info)
{
    node *q;
    q = new_node();
    q->info = info;
    
    q->next = (*first);           
    (*first) = q; 
}

//pop obtiene el elemento de la pila
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