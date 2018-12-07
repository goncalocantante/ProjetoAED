#ifndef _PROBLEMAS_H
#define _PROBLEMAS_H

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

typedef struct _vertex{
  int key;
  int x,y;
}vertex;

typedef struct {
  int nlinhas;
  int ncolunas;
  char modo;
  int npontos;
  //int **pontos;
  vertex *pontos;
  int **mapa;
}problema;

typedef struct {
  int valido;
  int custo;
  int *n_passos;
  vertex **passos;
}solucao;

 typedef struct _HeapNode{
  int key;
  vertex coord;
} HeapNode;

int CompareKey(Item, Item);
Item CreateHeapNode(int , int , int );

void printQueue(Heap *);
void InsertAll(problema, int, int, Heap *);
void Path_AtoB(vertex **, int **, problema, int, int, int, int, solucao *, int);
void InsertAndRelax_Adjs(Heap *, HeapNode *, int **, vertex **, problema);
int TrapedPoint(problema, int, int);

void RelaxEdge(Heap *, int **, vertex **, problema, HeapNode *, int, int);
void print_sol(FILE *, problema *, solucao *);

int DijkstraMagic(problema, int **, vertex **, int, int, int, int, solucao *, int *);
void InitSolution(solucao **, int);
problema *ler_problema(FILE *, vertex ***, int ***, solucao **);
int validate_problem(problema);
int validate_points(problema);
void free_problema(problema *, vertex **, int **, solucao *);
solucao *solve_problem(problema, vertex **, int **);
solucao *modoA(problema, vertex**, int **);
solucao *modoB(problema prob, vertex **, int **wt);

int SentCoordinates(Item, int);

#endif
