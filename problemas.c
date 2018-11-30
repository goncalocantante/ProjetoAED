#include <limits.h>

#include "utils.h"
#include "problemas.h"


problema *ler_problema(FILE *fp){
  int i = 0, j = 0, n_scan = 0;
  problema *prob = NULL;

  prob = (problema *)checked_malloc(sizeof(problema));

  //retira os dados da primeira linha do ficheiro
  n_scan = fscanf (fp,"%d %d %c %d", &prob->nlinhas ,&prob->ncolunas, &prob->modo, &prob->npontos);
  if (n_scan == EOF) {    //indica se há outro problema ou não
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


solucao *solve_problem(problema prob){
  solucao *sol = NULL;

  if (validate_problem(prob) == 0 || validate_points(prob) == 0) {
    sol = (solucao *)checked_malloc(sizeof(solucao));
    sol->valido = -1;
    sol->custo = 0;
  }
  else if (prob.modo == 'A') {
     modoA(prob);
  }
  return sol;
}

//algoritmo de Dijkstra
void modoA (problema prob){
  Heap** queue = NULL;
  int v, w, hsize, free = 0, x = prob.pontos[0][0], y = prob.pontos[0][1];
  int **wt, **st;

  //há possibilidade de fazer isto logo quando lemos o problema e evitamos fazer for for 
  hsize = prob.nlinhas*prob.ncolunas;
  //inicializa as matrizes do peso e do vértice anterior
  wt = (int **)checked_malloc(sizeof(int*)*prob.nlinhas);
  st = (int **)checked_malloc(sizeof(int*)*prob.nlinhas);
  for (v = 0; v < prob.nlinhas; v++){
    //acho que tem de se alocar para float o wt, mas faço isso depois
    wt[v] = (int*)checked_malloc(sizeof(int)*prob.ncolunas);
    st[v] = (int*)checked_malloc(sizeof(int)*prob.ncolunas);
    for (w = 0; w < prob.ncolunas; w++){
      st[v][w] = -1;
      wt[v][w] = INT_MAX/2; 
    }
  }
  //inicializa o Heap
  queue = HeapInit(hsize);
  //insere no heap todos os vértices adjacentes
  InsertAdjVert(prob, prob.pontos[0][0], prob.pontos[0][1], queue, free, hsize);
  wt[x][y] = 0;
  PriorityDec(x, y, queue, free);
  while(!EmptyHeap(queue)){
    
  }
}

//to be done
void InsertAdjVert(problema prob, int x, int y, Heap** queue, int free, int size){
  Heap *I = NULL;
  //verificar para cima
  if (x > 1){
    if (y < prob.ncolunas-1){ //verificar cima, direita
      if (prob.mapa[x-2][y+1] != 0) {   //verifica se a celula que vai aceder é acessivel
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x-2;
        I->y = y+1;
        HeapInsert (queue, I, free, size);
        
      }
    }
    if (y > 0){                     //verificar cima,esquerda
      if (prob.mapa[x-2][y-1] != 0) {
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x-2;
        I->y = y-1;
        HeapInsert (queue, I, free, size);
      }
    }
  }
  //verificar para baixo
  if (x < prob.nlinhas-2) {
    if (y < prob.ncolunas-1){ //verificar baixo, direita
      if (prob.mapa[x+2][y+1] != 0){
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x+2;
        I->y = y+1;
        HeapInsert (queue, I, free, size);
 
      }
    }
    if (y > 0){             //verificar baixo, esquerda
      if (prob.mapa[x+2][y-1] != 0) {
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x+2;
        I->y = y-1;
        HeapInsert (queue, I, free, size);
      }
    }
  }
  //verificar para esquerda
  if (y > 1){
    if (x < prob.nlinhas-1){ //verificar esquerda, baixo
      if (prob.mapa[x+1][y-2] != 0) {
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x+1;
        I->y = y-2;
        HeapInsert (queue, I, free, size);
      }
    }
    if (x > 0){               //verificar esquerda, cima
      if (prob.mapa[x-1][y-2] != 0) {
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x-11;
        I->y = y-2;
        HeapInsert (queue, I, free, size);
      }
    }
  }
  //verificar para a direita
  if (y < prob.ncolunas-2){
    if (x < prob.nlinhas-1){ //verificar direita, baixo
      if (prob.mapa[x+1][y+2] != 0) {
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x+1;
        I->y = y+2;
        HeapInsert (queue, I, free, size);
      }
    }
    if (x > 0){              //verificar direita, cima
      if (prob.mapa[x-1][y+2] != 0) {
        I = (Heap*)checked_malloc(sizeof(Heap));
        I->key = INT_MAX/2;
        I->x = x-1;
        I->y = y+2;
        HeapInsert (queue, I, free, size);
      }
    }
  }
 

}
