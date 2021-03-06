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
    q->prev = NULL;

    if(isEmpty((*first)))
 	{
        (*first) = q;
        (*last) = q;
    }
    else
   	{
        q->prev = *last;
        (*last)->next = q;
        (*last) = q;
    }
}

//insert at first position
void insert_at_first(node **first, node **last, data info)
{
    node *q;
    q = new_node();
    q->info = info;
    q->next = NULL;
    q->prev = NULL;

    if(isEmpty((*first)))
 	{
        (*first) = q;
        (*last) = q;
    }
    else
   	{
        q->prev = NULL;
        q->next = (*first);
        (*first) = q;
    }
}


void show_list(node **first)
{
    node* header = (*first);

    if(isEmpty(*first))
    {
        printf("\nLista vacia");
        return;
    }

    while(header)
    {
        printf("\nKey Nodo: %d", header->info.key);
        printf("\nName: %s", header->info.name);
        printf("\n--");
        header = header->next;
    }
}

void show_list_reverse(node **last)
{
    node* header = (*last);

    if(isEmpty(*last))
    {
        printf("\nLista vacia");
        return;
    }

    while(header)
    {
        printf("\nKey Nodo: %d", header->info.key);
        printf("\nName: %s", header->info.name);
        printf("\n--");
        header = header->prev;
    }
}

//find a link with given key
node* find_by_key(node **first, int key) 
{
    //start from the first link
    node *current = *first;
    if(isEmpty(*first))
    {
        return NULL;
    }

    //navigate through list
    while(current->info.key != key) 
    {
        //if it is last node
        if(current->next == NULL) 
        {
            return NULL;
        } 
        else
        {
            //go to next link
            current = current->next;
        }
    }              
    //if data found, return the current Link
    return current;
}

void delete_by_key(node **first, node **last, int key)
{
    //start from the first link
    node *current = *first;
    node *previous = *first;

    //navigate through list
    while(current != NULL && current->info.key != key) 
    {
        previous = current;
        current = current->next;
    }

    //No se encuentra el elemento en la lista
    if(current == NULL)
    {
        printf("\n No se encuentra el elemento de key %d", key);
        return;
    }

    //si es el primer elemento de la lista
    if(current == previous)
    {
        (*first) = (*first)->next;
        
        if(*first)
        {
            (*first)->prev = NULL;
        }
        else
        {
            //en este caso tendriamos la lista vacia
            *last = NULL;
        }
    }
    else
    {
        // Modijicar el enlace siguiente
        previous->next = current->next;
        //Modijicar el enlace anterior excepto para el ultimo
        if (current->next)
        {
            current->next->prev = current->prev;
        }
    }

    free(current);           
}

//delete all list
void clear(node **first, node **last)
{
     //start from the first link
    node *current = *first;
    node *toDelete;

    //navigate through list
    while(current != NULL) 
    {   
        toDelete = current;
        current = current->next;
        free(toDelete);
    }
    
    (*first) = NULL;
    (*last) = NULL;
}