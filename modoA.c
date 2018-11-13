//modoA.c

#include <stdio.h>
#include <stdlib.h>

#include "modos.h"
#include "tourist.h"

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
    A->custo = 0;
  }
  return A;
}
