#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//cria um novo nó e insere-o na cabeça da lista
LinkedList* InsertList(LinkedList *head, Vertice payload){
	LinkedList *new = (LinkedList*)checkedmalloc(sizeof(LinkedList));

	new->vertice = payload;
	if(head == NULL)
		return new;
	new->next = head;

	return new; 
}