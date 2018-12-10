#include "file.h"
#include "problemas.h"
#include "utils.h"

int main(int argc, char *argv[])
{
  FILE *fp_in = NULL, *fp_out = NULL;
  Problema *prob = NULL;
  Passeio *passeio = NULL;
  char *file_out = NULL;
  int **wt = NULL; 
  Vertex **st = NULL;
  
  //testa se o ficheiro é válido
  test_file(argv[1], argc);
  //abre o ficheiro para leitura
  fp_in = fopen(argv[1], "r");
  if (fp_in == NULL)
    exit(0);
  //gera-se o nome do ficheiro de saida e abre-se o ficheiro para escrita
  file_out = CreateOutputFileName(argv[1]);
  fp_out = fopen(file_out, "w");
  if (fp_out == NULL)
    exit(0);
  //enquanto houver problemas para resolver && e o Problema for válido
  while ((prob = ler_problema(fp_in, &st, &wt, &passeio)) != NULL)
  { 
    if (passeio == NULL){
      passeio = solve_problem(*prob, st, wt);
    }
    print_sol(fp_out, prob, passeio);
    free_problema(prob, st, wt, passeio);
    passeio = NULL;
    st = NULL;
    wt = NULL;
  }
  fclose(fp_in);
  fclose(fp_out);
  free(file_out);
  return 0;
}