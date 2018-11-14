#include <string.h>

#include "file.h"
#include "utils.h"

//verifica extensão (.cities) do programa
//verifica o número de argumentos da chamada do programa
void test_file (char nomefich[], int argc){
  char extensao[MAXSTR] = ".cities";
  char ext_fich[MAXSTR];
  int i, j = 0, len_ext = strlen(nomefich);

  for (i = (strlen(nomefich) - 7); i <= len_ext; i++) { // 7 = strlen de .cities
    ext_fich[j] = nomefich[i];
    j++;
  }

  if (argc != 2) {
    exit(0);
  }
  if (strcmp(ext_fich, extensao) != 0) {
    exit(0);
  }
}
