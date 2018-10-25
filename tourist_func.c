#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


int modoA (dados prob){
  int i, j, minimo = 5000;
  int x = prob.pontos[0][0], y = prob.pontos[0][1];

  //verificar para cima
  if (x > 1){
    if (y < prob.ncolunas-1){ //verificar desenho
      minimo = MIN( minimo, prob.mapa[x-2][y+1]);
    }
    if (y > 0){
      minimo = MIN( minimo, prob.mapa[x-2][y-1]);
    }
  }
  //verificar para baixo
  if (x < prob.nlinhas-2){
    if (y < prob.ncolunas-1){ //verificar desenho
      minimo = MIN( minimo, prob.mapa[x+2][y+1]);
    }
    if (y > 0){
      minimo = MIN( minimo, prob.mapa[x+2][y-1]);
    }
  }
  //verificar para esquerda
  if (y > 1){
    if (x < prob.nlinhas-1){ //verificar desenho
      minimo = MIN(minimo, prob.mapa[x+1][y-2]);
    }
    if (x > 0){
      minimo = MIN(minimo, prob.mapa[x-1][y-2]);
    }
  }
  //verificar para a direita
  if (y < prob.ncolunas-2){
    if (x < prob.nlinhas-1){ //verificar desenho
      minimo = MIN(minimo, prob.mapa[x+1][y+2]);
    }
    if (x > 0){
      minimo = MIN(minimo, prob.mapa[x-1][y+2]);
    }
  }
  return minimo;
}

//liberta memória alocada na estrutura
void free_struct(dados *estrutura){
  int i;

  for (i = 0; i < estrutura->ncolunas; i++) {
    free(estrutura->mapa[i]);
  }
  free(estrutura->mapa);
  for (i = 0; i < 2; i++) {
    free(estrutura->pontos[i]);
  }
  free(estrutura->pontos);
  free(estrutura);
}

//verifica extensão (.cities) do programa
//verifica o número de argumentos da chamada do programa
void test_file (char nomefich[], int argc){
  char extensao[MAXSTR] = ".cities";

  if (argc != 2) {
    exit(EXIT_FAILURE);
  }
  if (strstr(nomefich, extensao) == NULL) {
    exit(EXIT_FAILURE);
  }
}

//malloc com a verificação do erro
void* checked_malloc (size_t sz){
  void* mem = malloc(sz);
  if (mem == NULL){
    printf ("Out of memory!\n");
    exit(EXIT_FAILURE);
  }
  return mem;
}

dados *ler_problema(char nomefich[] ,FILE *fp){
  int i = 0, j = 0;
  dados *prob = NULL;

  prob = (dados *)checked_malloc(sizeof(dados));
  prob->modo = '\0';
  //retira os dados da primeira linha do ficheiro
  fscanf (fp, "%d %d %c %d", &prob->nlinhas ,&prob->ncolunas, &prob->modo, &prob->npontos);
  if (prob->modo != 'A' && prob->modo != 'B' && prob->modo != 'C') {     //indica se há outro problema ou não
    free(prob);
    return NULL;
  }
  //guarda coordenadas dos pontos numa matriz
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
