#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


int main(int argc, char *argv[]) {
  char modo;
  int **mapa = NULL;
  int **pontos = NULL;

  modo = lerficheiro(argv[1], mapa, pontos);

  printf("modo-%c\n", modo);

  printf("pontos: %d %d\n", pontos[0][0], pontos[0][1]);

  return(0);
}
