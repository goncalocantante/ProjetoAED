#ifndef _PROBLEMAS_H
#define _PROBLEMAS_H

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

typedef struct {
  int nlinhas;
  int ncolunas;
  char modo;
  int npontos;
  int **pontos;
  int **mapa;
}problema;

typedef struct {
  int valido;
  int custo;
}solucao;

typedef struct _vertex{
  int key;
  int x,y;  
}vertex;

void InsertAdjVert(problema, int , int , Heap *);
int CompareKey(Item, Item);
Item CreateVertex(int , int , int);
int FindIndex(Heap *, int, int);


problema *ler_problema(FILE *);
int validate_problem(problema);
int validate_points(problema);
void free_problema(problema *);
solucao *solve_problem(problema);
solucao *modoB(problema);
void modoA(problema);
//void InsertAdjVert(problema, int, int, Heap **, int *, int);


#endif
