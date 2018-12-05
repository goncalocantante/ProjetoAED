#include "file.h"
#include "problemas.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  FILE *fp_in = NULL, *fp_out = NULL;
  problema *prob = NULL;
  solucao *sol = NULL;
  char *file_out;
  int **wt = NULL, ***st = NULL; 

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
  while ((prob = ler_problema(fp_in, &st, &wt)) != NULL) {
    //printf("TAS COMNO\n");
    sol = solve_problem(fp_out, *prob, st, wt);
    print_sol(fp_out, prob, sol);
    // fprintf(fp_out, "\n");
    free_problema(prob, st, wt, sol);

  }
  fclose(fp_in);
  fclose(fp_out);
  return 0;
}