#ifndef GRAPH_H
#define  GRAPH_H

#include "utils.h"
#include "list.h"
#include "problemas.h"



typedef struct {
	int V;
	int E;
	LinkedList **ladjs;
} graph;

typedef struct Vertice{
	int V[2]; 
	int custo;
} Vertice;



graph* CreateGraph (problema);

#endif