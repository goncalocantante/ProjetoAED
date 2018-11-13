#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"


solucao *modoB (dados prob) {
  int i = 0, x, y, nx, ny;
  solucao *B;

  B = (solucao *)checked_malloc(sizeof(solucao));
  B->custo = prob.mapa[prob.pontos[0][0]][prob.pontos[0][1]];

  for (i = 0; i < prob.npontos - 1; i++) {
    B->valido = 0;
    x = prob.pontos[i][0];
    y = prob.pontos[i][1];
    nx = prob.pontos[i + 1][0];
    ny = prob.pontos[i + 1][1];

    if (x + 2 == nx && y + 1 == ny) {
      B->valido = 1;
    }
    if (x + 2 == nx && y - 1 == ny) {
      B->valido = 1;
    }
    if (x - 2 == nx && y + 1 == ny) {
      B->valido = 1;
    }
    if (x - 2 == nx && y - 1 == ny) {
      B->valido = 1;
    }

    if (y + 2 == ny && x + 1 == nx) {
      B->valido = 1;
    }
    if (y + 2 == ny && x - 1 == nx) {
      B->valido = 1;
    }
    if (y - 2 == ny && x + 1 == nx) {
      B->valido = 1;
    }
    if (y - 2 == ny && x - 1 == nx) {
      B->valido = 1;
    }

    if (B->valido == 0 || prob.mapa[x][y] == 0 || prob.mapa[nx][ny] == 0) {
      B->custo = 0;
      B->valido = 0;
      return B;
    }
    B->custo += prob.mapa[nx][ny];
  }
  return B;
}

solucao *modoA (dados prob){
  int x = prob.pontos[0][0], y = prob.pontos[0][1];
  solucao *A = NULL;
  A = (solucao *)checked_malloc(sizeof(solucao));
  A->valido = 1;
  A->custo = 5000;

  //verificar para cima
  if (x > 1){
    if (y < prob.ncolunas-1){ //verificar cima, direita
      if (prob.mapa[x-2][y+1] != 0) {    //verifica se a celula que vai aceder é acessivel
        A->custo = MIN( A->custo, prob.mapa[x-2][y+1]);
      }
    }
    if (y > 0){                     //verificar cima,esquerda
      if (prob.mapa[x-2][y-1] != 0) {
        A->custo = MIN( A->custo, prob.mapa[x-2][y-1]);
      }
    }
  }
  //verificar para baixo
  if (x < prob.nlinhas-2) {
    if (y < prob.ncolunas-1){ //verificar baixo, direita
      if (prob.mapa[x+2][y+1] != 0){
        A->custo = MIN( A->custo, prob.mapa[x+2][y+1]);
      }
    }
    if (y > 0){             //verificar baixo, esquerda
      if (prob.mapa[x+2][y-1] != 0) {
        A->custo = MIN( A->custo, prob.mapa[x+2][y-1]);
      }
    }
  }
  //verificar para esquerda
  if (y > 1){
    if (x < prob.nlinhas-1){ //verificar esquerda, baixo
      if (prob.mapa[x+1][y-2] != 0) {
        A->custo = MIN(A->custo, prob.mapa[x+1][y-2]);
      }
    }
    if (x > 0){               //verificar esquerda, cima
      if (prob.mapa[x-1][y-2] != 0) {
        A->custo = MIN(A->custo, prob.mapa[x-1][y-2]);
      }
    }
  }
  //verificar para a direita
  if (y < prob.ncolunas-2){
    if (x < prob.nlinhas-1){ //verificar direita, baixo
      if (prob.mapa[x+1][y+2] != 0) {
        A->custo = MIN(A->custo, prob.mapa[x+1][y+2]);
      }
    }
    if (x > 0){              //verificar direita, cima
      if (prob.mapa[x-1][y+2] != 0) {
        A->custo = MIN(A->custo, prob.mapa[x-1][y+2]);
      }
    }
  }
  if (A->custo == 5000) {
    A->valido = -1;
  }
  return A;
}

//liberta memória alocada na estrutura
void free_struct(dados *estrutura){
  int i;

  for (i = 0; i < estrutura->ncolunas; i++) {
    free(estrutura->mapa[i]);
  }
  free(estrutura->mapa);
  for (i = 0; i < estrutura->npontos; i++) {
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
