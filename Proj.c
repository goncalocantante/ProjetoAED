#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


int main(int argc, char *argv[]) {
  FILE *fp = NULL;
  dados *prob;
  int i, j, resultadoA;

  test_file(argv[1], argc);

  fp = fopen(argv[1], "r");
  if (fp == NULL) exit(EXIT_FAILURE);
  // TODO abrir ficheiro de saida

  while ((prob = ler_problema(argv[1], fp)) != NULL) {

    resultadoA = modoA(*prob);

    printf("resultado %d\n", resultadoA);

  //   //TODO escrever
    free_struct(prob);
  }
  //   //TODO fechar os dois ficheiros
  fclose(fp);

  return(0);
}
