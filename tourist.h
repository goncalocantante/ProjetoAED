#ifndef _TOURIST_H
#define _TOURIST_H

#include <stdio.h>
#include <stdlib.h>

#define MAXSTR 100

typedef struct {
  int nlinhas;
  int ncolunas;
  char modo;
  int npontos;
  int **pontos;
  int **mapa;
}dados;

dados *ler_problema (char [] , FILE *);
void* checked_malloc (size_t);
void test_file (char [], int);


#endif
