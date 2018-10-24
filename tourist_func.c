#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


void testfile (char nomefich[], int argc){
  char extensao[MAXSTR] = ".cities";

  if (argc != 2) {
    exit(EXIT_FAILURE);
  }
  if (strstr(nomefich, extensao) == NULL) {
    exit(EXIT_FAILURE);
  }
}

void* checkedmalloc (size_t sz){
  void* mem = malloc(sz);
  if (mem == NULL){
    printf ("Out of memory!\n");
    exit(EXIT_FAILURE);
  }
  return mem;
}

char lerficheiro (char nomefich[] ,int ***mat, int  ***pontos, FILE *fp, int *linhas, int *colunas, int *npontos){
  // FILE *fp = NULL;
  int linhas = 0, colunas = 0, npont = 0;
  int i = 0, j = 0;
  char modo = 'D';

  // fp = fopen(nomefich, "r");
  // if (fp == NULL) exit(EXIT_FAILURE);

  //retira informações necessárias da primeira linha do ficheiro
  fscanf (fp, "%d %d %c %d", &linhas ,&colunas, &modo, &npont);
  if (modo != 'A' && modo != 'B' && modo != 'C') {
    return '0';
  }

  //matriz com os pontos de interesse
  *pontos = (int**)checkedmalloc(sizeof(int*)*npont);
  for (i = 0; i < npont; i++){
      (*pontos)[i] = (int*)checkedmalloc(sizeof(int)*2);
      fscanf(fp, "%d %d", &((*pontos)[i][0]), &((*pontos)[i][1]));
  }
  printf("pontos: %d %d\n", (*pontos)[0][0], (*pontos)[0][1]);


  //matriz do mapa da cidade
  *mat = (int**)checkedmalloc(sizeof(int*)*linhas);

  for (i = 0; i < linhas; i++){
    (*mat)[i] = (int*)checkedmalloc(sizeof(int)*colunas);
    for (j = 0; j < colunas; j++){
      fscanf(fp,"%d", &(*mat)[i][j]);
    }
  }

  return modo;
}
