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

dados *lerficheiro (char [] , FILE *);
void* checkedmalloc (size_t);
void testfile (char [], int);


#endif
