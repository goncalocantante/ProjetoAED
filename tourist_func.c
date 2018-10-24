#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


char lerficheiro (char nomefich[] ,int** mat, int* pontos[2]){
  FILE *fp = NULL;
  int linhas = 0, colunas = 0, npont = 0;
  char modo = 'D';

  fp = fopen(fp, nomefich, "r");
  if (fp == NULL) exit(EXIT_FAILURE);

  //retira informações necessárias da primeira linha do ficheiro
  fscanf (fp, "%d %d %c %d", &linhas ,&colunas, &modo, &npont);

}
