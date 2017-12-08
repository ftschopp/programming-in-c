/*
 *ftschopp Lista doblemente enlazada
 * 
 */

#ifndef _LISTA_H_
#define _LISTA_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

typedef struct data{
	int key;
	char name[30];
	int connfd;
	struct sockaddr_in addr;
	
}clientData;

typedef struct node {
 	clientData info;
	struct node *next; //next node
	struct node *prev; //previous node
 }node;


 //declaracion de funciones de lista
node* new_node();
//Insert a new node in a list
void insert(node **first, node **last, clientData info);
void insert_at_first(node **first, node **last, clientData info);
//check if list is empty
int isEmpty(node* t);
//show list
void show_list(node **first);
//show list in reverse mode
void show_list_reverse(node **last);
// find node by key
node* find_by_key(node **first, int key); 
//delete node by key
void delete_by_key(node **first, node **last, int key);
//delete all list
void clear(node **first, node **last);
//Count of nodes
int getCount(node *first);
#endif