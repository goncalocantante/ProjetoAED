#include <string.h>

#include "file.h"
#include "utils.h"

//verifica extensão (.cities) do programa
//verifica o número de argumentos da chamada do programa
void test_file(char nomefich[], int argc)
{
  int i, j = 0, len = strlen(nomefich), len_ext = strlen(".cities");
  char *extensao = (char *)checked_malloc(sizeof(char) * len_ext + 1);
  char *ext_fich = (char *)checked_malloc(sizeof(char) * len_ext + 1);
  
  //verifica numero de argumentos
  if (argc != 2)
  {
    exit(0);
  }
  //extensão correta
  extensao = strcpy(extensao, ".cities");
  //extensão do ficheiro usado como argumento
  for (i = len - len_ext; i <= len; i++)
  {
    ext_fich[j] = nomefich[i];
    j++;
  }
  //verifica a extensão do ficheiro
  if (strcmp(ext_fich, extensao) != 0)
  {
    exit(0);
  }
  free(extensao);
  free(ext_fich);
}

char *CreateOutputFileName(char *raw_name)
{
  char *file_out = (char *)checked_malloc(sizeof(char) * strlen(raw_name) + 1);
  int i, len = strlen(raw_name);

  strcpy(file_out, raw_name);
  for (i = len - strlen(".cities"); i <= len; i++)
  { // 7 = strlen de .cities
    file_out[i] = '\0';
  }
  strcat(file_out, ".walks");

  return file_out;
}