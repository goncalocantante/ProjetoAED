#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


int main(int argc, char *argv[]) {
  dados *prob;
  int i, j;

  FILE *fp = NULL;

  test_file(argv[1], argc);

  fp = fopen(argv[1], "r");
  if (fp == NULL) exit(EXIT_FAILURE);

  while ((prob = ler_problema(argv[1], fp)) != NULL) {

    printf("Linhas %d\nColunas %d\nModo %c\nNpontos %d\n", prob->nlinhas, prob->ncolunas, prob->modo, prob->npontos);
    printf("pontos: %d %d\n", prob->pontos[0][0], prob->pontos[0][1]);
    printf("pontos: %d %d\n", prob->pontos[1][0], prob->pontos[1][1]);

    for (i = 0; i < prob->nlinhas; i++){
      for (j = 0; j < prob->ncolunas; j++){
        printf("%d ", prob->mapa[i][j]);
      }
      printf("\n");
    }
    printf("Acabou um problema\n");
  //   //TODO escrever
  //   //TODO fechar
    for (i = 0; i < 11; i++) {
      free(prob->mapa[i]);
    }
    free(prob->mapa);
    for (i = 0; i < 2; i++) {
      free(prob->pontos[i]);
    }
    free(prob->pontos);
  }

  free(prob);

  // if (feof(fp)) {
  //   printf("O ficheiro avabou\n");
  // }

  return(0);
}
