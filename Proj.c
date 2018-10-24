#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


int main(int argc, char *argv[]) {
  char modo;
  int **mapa = NULL;
  int **pontos = NULL;
  int i, j;

  FILE *fp = NULL;

  testfile(argv[1], argc);

  fp = fopen(argv[1], "r");
  if (fp == NULL) exit(EXIT_FAILURE);



  modo = lerficheiro(argv[1], &mapa, &pontos, fp);

  printf("modo-%c\n", modo);

  printf("pontos: %d %d\n", pontos[0][0], pontos[0][1]);
  printf("\n");
  for (i = 0; i < 10; i++){
    for (j = 0; j < 11; j++){
      printf("%d ", mapa[i][j]);
    }
    printf("\n");
  }
  //TODO escrever
  //TODO fechar
  //TODO free

  // if (feof(fp)) {
  //   printf("O ficheiro avabou\n");
  // }

  return(0);
}
