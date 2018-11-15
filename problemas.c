
#include "problemas.h"
#include "utils.h"


problema *ler_problema(FILE *fp){
  int i = 0, j = 0, n_scan = 0;
  problema *prob = NULL;

  prob = (problema *)checked_malloc(sizeof(problema));

  //retira os dados da primeira linha do ficheiro
  n_scan = fscanf (fp, "%d %d %c %d", &prob->nlinhas ,&prob->ncolunas, &prob->modo, &prob->npontos);
  if (n_scan == EOF) {     //indica se há outro problema ou não
    free(prob);
    return NULL;
  }
  //guarda coordenadas dos pontos numa matriz
  prob->pontos = (int**)checked_malloc(sizeof(int*)*prob->npontos);
  for (i = 0; i < prob->npontos; i++){
      prob->pontos[i] = (int*)checked_malloc(sizeof(int)*2);
      n_scan = fscanf(fp, "%d %d", &prob->pontos[i][0], &prob->pontos[i][1]);
  }
  //matriz do mapa da cidade
  prob->mapa = (int**)checked_malloc(sizeof(int*)*prob->nlinhas);
  for (i = 0; i < prob->nlinhas; i++){
    prob->mapa[i] = (int*)checked_malloc(sizeof(int)*prob->ncolunas);
    for (j = 0; j < prob->ncolunas; j++){
      n_scan = fscanf(fp,"%d", &prob->mapa[i][j]);
    }
  }

  return prob;
}

int validate_problem(problema prob){
  int ex_valido = 1;

  if ((prob.modo != 'A' && prob.modo != 'B') || prob.nlinhas <= 0 || prob.ncolunas <= 0
      || prob.npontos <= 0 || (prob.modo == 'A' && prob.npontos != 1)) {
        ex_valido = 0;
  }
  return ex_valido;
}

//verifica se todos os pontos são acessiveis
int validate_points(problema prob){
  int i = 0, ponto_valido = 1;

  for (i = 0; i < prob.npontos; i++) {
    if ((prob.pontos[i][0] < 0) || (prob.pontos[i][0] >= prob.nlinhas) || (prob.pontos[i][1] >= prob.ncolunas)) {
      ponto_valido = 0;
      i = prob.npontos;
      return ponto_valido;
    }
    if (prob.mapa[prob.pontos[i][0]][prob.pontos[i][1]] == 0){
      ponto_valido = 0;
      i = prob.npontos;
      return ponto_valido;
    }
  }
  return ponto_valido;
}

//liberta memória alocada na estrutura
void free_problema(problema *estrutura){
  int i;

  for (i = 0; i < estrutura->nlinhas; i++) {
    free(estrutura->mapa[i]);
  }
  free(estrutura->mapa);
  for (i = 0; i < estrutura->npontos; i++) {
    free(estrutura->pontos[i]);
  }
  free(estrutura->pontos);
  free(estrutura);
}

solucao *solve_problem(problema prob) {
  solucao *sol = NULL;

  if (validate_problem(prob) == 0 || validate_points(prob) == 0) {
    sol = (solucao *)checked_malloc(sizeof(solucao));
    sol->valido = -1;
    sol->custo = 0;
  }
  else if (prob.modo == 'A') {
    sol = modoA(prob);
  }
  else if (prob.modo == 'B') {
    sol = modoB(prob);
  }
  return sol;
}

solucao *modoB (problema prob) {
  int i = 0, x, y, nx, ny;
  solucao *B;

  B = (solucao *)checked_malloc(sizeof(solucao));
  B->custo = 0;
  B->valido = -1;

  for (i = 0; i < prob.npontos - 1; i++) {
    B->valido = -1;
    x = prob.pontos[i][0];
    y = prob.pontos[i][1];
    nx = prob.pontos[i + 1][0];
    ny = prob.pontos[i + 1][1];

    //baixo direita
    if ((x + 2) == nx && (y + 1) == ny) {
      B->valido = 1;
    }
    //baixo esquerda
    if ((x + 2) == nx && (y - 1) == ny) {
      B->valido = 1;
    }
    //cima direita
    if ((x - 2) == nx && (y + 1) == ny) {
      B->valido = 1;
    }
    //cima esquerda
    if ((x - 2) == nx && (y - 1) == ny) {
      B->valido = 1;
    }
    //direita baixo
    if ((y + 2) == ny && (x + 1) == nx) {
      B->valido = 1;
    }
    //direita cima
    if ((y + 2) == ny && (x - 1) == nx) {
      B->valido = 1;
    }
    //esquerda baixo
    if ((y - 2) == ny && (x + 1) == nx) {
      B->valido = 1;
    }
    //esquerda cima
    if ((y - 2) == ny && (x - 1) == nx) {
      B->valido = 1;
    }
    //valida a jogada
    if (B->valido == -1 || prob.mapa[x][y] == 0 || prob.mapa[nx][ny] == 0) {
      B->custo = 0;
      B->valido = -1;
      return B;
    }
    B->custo += prob.mapa[nx][ny];
  }
  return B;
}

solucao *modoA (problema prob){
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
