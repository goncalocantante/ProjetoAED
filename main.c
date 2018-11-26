#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "problemas.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  FILE *fp_in = NULL;
  FILE *fp_out = NULL;
  problema *prob = NULL;
  solucao *sol = NULL;
  char file_out[MAXSTR];
  int i = 0;
  int len_ext = strlen(argv[1]);

  //testa se o ficheiro é válido
  test_file(argv[1], argc);
  strcpy(file_out, argv[1]);

  fp_in = fopen(argv[1], "r");
  if (fp_in == NULL) exit(0);
  
  for (i = strlen(argv[1]) - 7; i <= len_ext; i++) { // 7 = strlen de .cities
    file_out[i] = '\0';
  }
  strcat(file_out,".valid");
  fp_out = fopen(file_out, "w");
  if (fp_out == NULL) exit(0);
  
  while ((prob = ler_problema(fp_in)) != NULL) {

    sol = solve_problem(*prob);
    fprintf(fp_out,"%d %d %c %d %d %d\n", prob->nlinhas, prob->ncolunas, prob->modo, prob->npontos, sol->valido, sol->custo);
    free(sol);

    fprintf(fp_out, "\n");
    free_problema(prob);
  }
  fclose(fp_in);
  fclose(fp_out);

  return 0;
}
