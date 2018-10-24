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

char lerficheiro (char nomefich[] ,int ***mat, int  ***pontos, FILE *fp){
  // FILE *fp = NULL;
  int linhas = 0, colunas = 0, npont = 0;
  int i = 0, j = 0;
  char modo = 'D';
  char buffer[MAXSTR];

  // fp = fopen(nomefich, "r");
  // if (fp == NULL) exit(EXIT_FAILURE);

  //retira informações necessárias da primeira linha do ficheiro
  fgets(buffer, 10, fp);
  sscanf (buffer, "%d %d %c %d", &linhas ,&colunas, &modo, &npont);
  printf("%d, %d, %c, %d\n", linhas, colunas, modo, npont);


  //matriz com os pontos de interesse
  *pontos = (int**)checkedmalloc(sizeof(int*)*npont);
  for (i = 0; i < npont; i++){
      (*pontos)[i] = (int*)checkedmalloc(sizeof(int)*2);
      fgets(buffer, 100, fp);
      printf("%s\n", buffer);
      sscanf(buffer, "%d %d", &((*pontos)[i][0]), &((*pontos)[i][1]));
  }
  printf("pontos: %d %d\n", (*pontos)[0][0], (*pontos)[0][1]);


  //matriz do mapa da cidade
  *mat = (int**)checkedmalloc(sizeof(int*)*linhas);

  for (i = 0; i < linhas; i++){
    (*mat)[i] = (int*)checkedmalloc(sizeof(int)*colunas);
    for (j = 0; j < colunas; j++){
      fgets(buffer, 100, fp);
      sscanf(buffer,"%d", &(*mat)[i][j]);
    }
  }
  fgets(buffer, 100, fp); //mete na string buffer a linha em branco

  // printf("%s\n", tes);
  return modo;
}
