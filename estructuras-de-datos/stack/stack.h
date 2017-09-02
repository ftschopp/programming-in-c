/*
 *ftschopp pila 
 * 
 */

#ifndef _STACK_H_
#define _STACK_H_


typedef struct data{
	int key;
	char name[30];
}data;

typedef struct node {
 	data info;
	struct node *next; //next node
 }node;


//declaracion de funciones de la pila
node* new_node();

//check if stack is empty
int isEmpty(node* t);

//Insert a new node in a stack
void push(node **first, data info);

//Remove front node of a stack
node* pop(node **first);

#endif