#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tourist.h"

int main(int argc, char *argv[]) {
  FILE *fp_in = NULL;
  FILE *fp_out = NULL;
  dados *prob = NULL;
  solucao *B = NULL, *A = NULL;
  char file_out[MAXSTR];
  int i = 0;

  strcpy(file_out, argv[1]);

  test_file(argv[1], argc);

  fp_in = fopen(argv[1], "r");
  if (fp_in == NULL) exit(0);

  for (i = strlen(argv[1]) - 7; i <= strlen(argv[1]); i++) { // 7 = strlen de .cities
    file_out[i] = '\0';
  }
  strcat(file_out,".walks");
  fp_out = fopen(file_out, "w");
  if (fp_out == NULL) exit(0);

  while ((prob = ler_problema(argv[1], fp_in)) != NULL) {

    if (prob->modo == 'A') {
      A = modoA(*prob);
      fprintf(fp_out,"%d %d %c %d %d\n", prob->nlinhas, prob->ncolunas, prob->modo, A->valido, A->custo);
      free(A);
    }
    else if (prob->modo == 'B') {
      B = modoB(*prob);
      fprintf(fp_out,"%d %d %c %d %d\n", prob->nlinhas, prob->ncolunas, prob->modo, B->valido, B->custo);
      free(B);
    }

    fprintf(fp_out, "\n");
    free_struct(prob);
  }
  fclose(fp_in);
  fclose(fp_out);

  return(0);
}
