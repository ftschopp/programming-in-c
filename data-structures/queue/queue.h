/*
 *ftschopp cola simplemente enlazada
 * 
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_


typedef struct data{
	int key;
	char name[30];
}data;

typedef struct node {
 	data info;
	struct node *next; //next node
 }node;


//declaracion de funciones de cola
node* new_node();

//check if queue is empty
int isEmpty(node* t);

//Insert a new node in a queue
void push(node **first, node **last, data info);

//Remove front node of a queue
node* pop(node **first);

#endif