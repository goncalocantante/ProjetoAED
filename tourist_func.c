#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


void test_file (char nomefich[], int argc){
  char extensao[MAXSTR] = ".cities";

  if (argc != 2) {
    exit(EXIT_FAILURE);
  }
  if (strstr(nomefich, extensao) == NULL) {
    exit(EXIT_FAILURE);
  }
}

void* checked_malloc (size_t sz){
  void* mem = malloc(sz);
  if (mem == NULL){
    printf ("Out of memory!\n");
    exit(EXIT_FAILURE);
  }
  return mem;
}

dados *ler_problema(char nomefich[] ,FILE *fp){
  // FILE *fp = NULL;
  int i = 0, j = 0;
  dados *prob;

  // fp = fopen(nomefich, "r");
  // if (fp == NULL) exit(EXIT_FAILURE);

  prob = (dados *)checked_malloc(sizeof(dados));

  //retira informações necessárias da primeira linha do ficheiro
  fscanf (fp, "%d %d %c %d", &prob->nlinhas ,&prob->ncolunas, &prob->modo, &prob->npontos);
  if (prob->modo != 'A' && prob->modo != 'B' && prob->modo != 'C') {     //indica se há outro problema ou não
    return NULL;
  }

  //matriz com os pontos de interesse
  prob->pontos = (int**)checked_malloc(sizeof(int*)*prob->npontos);
  for (i = 0; i < prob->npontos; i++){
      prob->pontos[i] = (int*)checked_malloc(sizeof(int)*2);
      fscanf(fp, "%d %d", &prob->pontos[i][0], &prob->pontos[i][1]);
  }


  //matriz do mapa da cidade
  prob->mapa = (int**)checked_malloc(sizeof(int*)*prob->nlinhas);

  for (i = 0; i < prob->nlinhas; i++){
    prob->mapa[i] = (int*)checked_malloc(sizeof(int)*prob->ncolunas);
    for (j = 0; j < prob->ncolunas; j++){
      fscanf(fp,"%d", &prob->mapa[i][j]);
    }
  }

  return prob;
}
