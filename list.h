#ifndef LIST_H
#define LIST_H

#include "graph.h" 

typedef struct {
	Vertice *vertice;
	LinkedList *next;
} LinkedList;



LinkedList* InsertList(LinkedList *, Vertice *);


#endif