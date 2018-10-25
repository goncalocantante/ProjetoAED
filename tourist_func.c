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

dados *lerficheiro (char nomefich[] ,FILE *fp){
  // FILE *fp = NULL;
  int i = 0, j = 0;
  dados *prob;
  char modo = 'D';

  // fp = fopen(nomefich, "r");
  // if (fp == NULL) exit(EXIT_FAILURE);

  prob = (dados *)checkedmalloc(sizeof(dados));

  //retira informações necessárias da primeira linha do ficheiro
  fscanf (fp, "%d %d %c %d", &prob->nlinhas ,&prob->ncolunas, &prob->modo, &prob->npontos);
  // if (modo != 'A' && modo != 'B' && modo != 'C') {
  //   return '0';
  // }

  //matriz com os pontos de interesse
  prob->pontos = (int**)checkedmalloc(sizeof(int*)*prob->npontos);
  for (i = 0; i < prob->npontos; i++){
      prob->pontos[i] = (int*)checkedmalloc(sizeof(int)*2);
      fscanf(fp, "%d %d", &prob->pontos[i][0], &prob->pontos[i][1]);
  }

  // printf("pontos: %d %d\n", prob->pontos[0][0], prob->pontos[0][1]);
  // printf("pontos: %d %d\n", prob->pontos[1][0], prob->pontos[1][1]);


  // //matriz do mapa da cidade
  // *mat = (int**)checkedmalloc(sizeof(int*)*linhas);
  //
  // for (i = 0; i < linhas; i++){
  //   (*mat)[i] = (int*)checkedmalloc(sizeof(int)*colunas);
  //   for (j = 0; j < colunas; j++){
  //     fscanf(fp,"%d", &(*mat)[i][j]);
  //   }
  // }

  return prob;
}
