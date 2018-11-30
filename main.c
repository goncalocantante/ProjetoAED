#include "file.h"
#include "problemas.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  FILE *fp_in = NULL, *fp_out = NULL;
  problema *prob = NULL;
  //solucao *sol = NULL;
  char *file_out;

  //testa se o ficheiro é válido
  test_file(argv[1], argc);
  //abre o ficheiro para leitura
  fp_in = fopen(argv[1], "r");
  if (fp_in == NULL) exit(0);
  //gera-se o nome do ficheiro de saida e abre-se o ficheiro para escrita
  file_out = CreateOutputFileName(argv[1]);
  fp_out = fopen(file_out, "w");
  if (fp_out == NULL) exit(0);
  //enquanto houver problemas para resolver
  while ((prob = ler_problema(fp_in)) != NULL) {
    /*sol = solve_problem(*prob);
    fprintf(fp_out,"%d %d %c %d %d %d\n", prob->nlinhas, prob->ncolunas, prob->modo, prob->npontos, sol->valido, sol->custo);
    free(sol);
    fprintf(fp_out, "\n");*/



    free_problema(prob);
  }
  fclose(fp_in);
  fclose(fp_out);
  return 0;
}