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
  int *n_passos;
  struct _vertex **passos;
}solucao;

typedef struct _vertex{
  int key;
  int x,y;  
}vertex;


int CompareKey(Item, Item);
Item CreateVertex(int , int , int);
int FindIndex(Heap *, int, int);
void printQueue(Heap *);
void InsertAll(problema , int , int , Heap *);
void Path_AtoB(int ***, int **, problema , int , int , int , int, solucao *, int); 
void InsertAndRelax_Adjs(Heap *, vertex *, int **, int ***, problema);
int TrapedPoint(problema, int, int);

void RelaxEdge(Heap *, int **, int ***, problema, vertex *, int, int);
void print_sol(FILE *, problema *, solucao *);

int DijkstraMagic(problema, int **, int ***, int, int, int, int);
void InitSolution(solucao **, int);
problema *ler_problema(FILE *, int ****, int ***);
int validate_problem(problema);
int validate_points(problema);
void free_problema(problema *, int ***, int **, solucao *);
solucao *solve_problem(FILE *, problema, int ***, int **);
solucao *modoA(problema, int ***, int **);
solucao *modoB(problema prob, int ***st, int **wt);

int SentCoordinates(Item, int);

#endif
