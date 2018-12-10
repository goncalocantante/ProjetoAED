#ifndef _PROBLEMAS_H
#define _PROBLEMAS_H

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

typedef struct _vertex{
  int x,y;
}Vertex;

typedef struct {
  int custo;
  int n_passos;
  Vertex *passos;
}Passo;

typedef struct {
  int nlinhas;
  int ncolunas;
  char modo;
  int npontos;
  Vertex *pontos;
  int **mapa;
}Problema;

typedef struct
{
  int CustoTotal;   //um passeio tem o custo total
  int n_passos;
  Passo **passos;   // e vários passos (ventor de ponteiros para Passo)
} Passeio;

typedef struct _HeapNode
{
  int key;
  Vertex coord;
} HeapNode;

int CompareKey(Item, Item);
Item CreateHeapNode(int , int , int );

void printQueue(Heap *);
void InsertAll(Problema, int, int, Heap *);
void Path_AtoB(Vertex **, int **, Problema , int , int , int , int , Passo **);

void InsertAndRelax_Adjs(Heap *, HeapNode *, int **, Vertex **, Problema);
int TrapedPoint(Problema, int, int);

void RelaxEdge(Heap *, int **, Vertex **, Problema, HeapNode *, int, int);
void print_sol(FILE *, Problema *, Passeio *);

void DijkstraMagic(Problema, int **, Vertex **, int, int, HeapNode* , int);
void InitPasseio(Passeio **, int);
void InitPasso (Passo **, int );


Problema *ler_problema(FILE *, Vertex ***, int ***, Passeio **);
int validate_problem(Problema);
int validate_points(Problema);
void free_problema(Problema *, Vertex **, int **, Passeio *);
Passeio *solve_problem(Problema, Vertex **, int **);
Passo *modoA(Problema , Vertex **, int **);
Passeio *modoB(Problema, Vertex **, int **);
Passeio *modoC(Problema, Vertex **, int **);

int SentCoordinates(Item, int);

int StopDijkstra(HeapNode *, int, int, int);
void printArr(int *, int); ///Melhora para o Dijkstra e Maquina de Permutações
void PermutationBeast(int *a, int size, int n);
#endif
