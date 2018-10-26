#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


int main(int argc, char *argv[]) {
  FILE *fp = NULL;
  dados *prob = NULL;
  solucao *B = NULL, *A = NULL;

  test_file(argv[1], argc);

  fp = fopen(argv[1], "r");
  if (fp == NULL) exit(EXIT_FAILURE);
  // TODO abrir ficheiro de saida

  while ((prob = ler_problema(argv[1], fp)) != NULL) {

    if (prob->modo == 'A') {
      A = modoA(*prob);
      printf("resultado %d\ncusto %d\n", A->valido, A->custo);
    }
    else if (prob->modo == 'B') {
      B = modoB(*prob);
      printf("resultado %d\ncusto %d\n", B->valido, B->custo);
    }

    printf("\n");
    //TODO escrever
    free_struct(prob);
    free(A);
    free(B);
  }
  //   //TODO fechar os dois ficheiros
  fclose(fp);

  return(0);
}
