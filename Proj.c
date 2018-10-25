#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


int main(int argc, char *argv[]) {
  char modo;
  dados *prob;
  int i, j;

  FILE *fp = NULL;

  testfile(argv[1], argc);

  fp = fopen(argv[1], "r");
  if (fp == NULL) exit(EXIT_FAILURE);


  prob = lerficheiro(argv[1], fp);

  printf("Linhas %d\nColunas %d\nModo %c\nNpontos %d\n", prob->nlinhas, prob->ncolunas, prob->modo, prob->npontos);

  // while ((modo = lerficheiro(argv[1], prob)) != '0') {
  //   printf("modo-%c\n", modo);
  //
    printf("pontos: %d %d\n", prob->pontos[0][0], prob->pontos[0][1]);
    printf("pontos: %d %d\n", prob->pontos[1][0], prob->pontos[1][1]);
  //   printf("\n");
  //   for (i = 0; i < 10; i++){
  //     for (j = 0; j < 11; j++){
  //       printf("%d ", mapa[i][j]);
  //     }
  //     printf("\n");
  //   }
  //   //TODO escrever
  //   //TODO fechar
  //   for (i = 0; i < 11; i++) {
  //     free(mapa[i]);
  //   }
  //   free(mapa);
  //   for (i = 0; i < 2; i++) {
  //     free(pontos[i]);
  //   }
  //   free(pontos);
  //
  // }

  // if (feof(fp)) {
  //   printf("O ficheiro avabou\n");
  // }

  return(0);
}
