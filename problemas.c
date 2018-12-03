#include <limits.h>
#include <stdbool.h>

#include "utils.h"
#include "problemas.h"

//Para o vetor das coordenadas dos pontos adjacentes
#define Left2Up 0
#define Left2Down 1
#define Right2Up 2
#define Right2Down 3
#define Up2Left 4
#define Up2Right 5
#define Down2Left 6
#define Down2Right 7


problema *ler_problema(FILE *fp, int ****st, int ***wt){
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
  //vetores auxiliares para algoritmo de Dijkstra
  (*wt) = (int **)checked_malloc(sizeof(int *) * prob->nlinhas);
  (*st) = (int ***)checked_malloc(sizeof(int **) * prob->nlinhas);
  for (i = 0; i < prob->nlinhas; i++){
    prob->mapa[i] = (int*)checked_malloc(sizeof(int)*prob->ncolunas);
    (*wt)[i] = (int *)checked_malloc(sizeof(int) * prob->ncolunas);
    (*st)[i] = (int **)checked_malloc(sizeof(int *) * prob->ncolunas);
    for (j = 0; j < prob->ncolunas; j++){
      n_scan = fscanf(fp,"%d", &prob->mapa[i][j]);
      (*st)[i][j] = (int *)checked_malloc(sizeof(int) * 2);
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
void free_problema(problema *estrutura, int ***st, int **wt){
  int i, j;

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

solucao *solve_problem(problema prob, int ***st, int **wt ){
  solucao *sol = NULL;

  if (validate_problem(prob) == 0 || validate_points(prob) == 0) {
    sol = (solucao *)checked_malloc(sizeof(solucao));
    sol->valido = -1;
    sol->custo = 0;
  }
  //else if (prob.modo == 'A') {
     modoA(prob, st, wt); 
  //}
  return sol;

}

//Encontra o melhor caminho entre A e B
//Retorna o numero de pontos do caminho
void DijkstraMagic(problema prob, int **wt, int ***st,int Xa, int Ya, int Xb, int Yb){
  Heap *heap = NULL;
  vertex *V;
  int x, y, VertsAdjs[2][8], count = 0;
  solucao *sol;

  printf("TAS COMNO\n");
  //inicializa os vetores auxiliares
  for (int v = 0; v < prob.nlinhas; v++){
    for (int w = 0; w < prob.ncolunas; w++){
      //[v][w][0] = x  || [v][w][1] = y
      st[v][w][0] = -1;
      st[v][w][1] = -1;
      wt[v][w] = INT_MAX / 2;
    }
  }
  //inicializa o Heap
  heap = HeapInit(CompareKey, (prob.ncolunas * prob.nlinhas));
  //insere no heap todos os vértices
  InsertAll(prob, prob.nlinhas, prob.ncolunas, heap);

  //altera a prioridade
  wt[Xa][Ya] = 0;
  ChangePri(heap, FindIndex(heap, Xa, Ya), CreateVertex(Xa, Ya, 0));

  while (EmptyHeap(heap) == 0)
  {
    V = getMostPri(heap);
    if (V->key != INT_MAX / 2)
    {
      GetAdjs(prob, V->x, V->y, VertsAdjs);
      //ve todos os adjacentes
      for (int i = 0; i < 8; i++)
      {
        //se as coordenadas do vertice adjacente forem válidas
        if (VertsAdjs[0][i] != -5)
        {
          //coordenadas do vértice adjacente
          x = V->x + VertsAdjs[0][i];
          y = V->y + VertsAdjs[1][i];
          //relaxa a aresta
          if (wt[x][y] > (V->key + prob.mapa[x][y])) //isto está correto tmb
          {
            wt[x][y] = V->key + prob.mapa[x][y];
            //atualizar o st
            st[x][y][0] = V->x;
            st[x][y][1] = V->y;
            //o indice está correto
            ChangePri(heap, FindIndex(heap, x, y), CreateVertex(x, y, wt[x][y]));
          }
        }
      }
    }
    HeapDeleteMostPri(heap);
  }
  free(getHeapData(heap));
  free(heap);
  Path_AtoB(st, prob, Xb, Yb, Xa, Ya, &sol, &count);
  sol->custo = wt[Xb][Yb];
}


void modoA (problema prob, int ***st, int **wt){

  DijkstraMagic(prob, wt, st, prob.pontos[0][0], prob.pontos[0][1], prob.pontos[1][0], prob.pontos[1][1]);
}

Item CreateVertex(int x, int y, int key){ 
  vertex * vert = (vertex*)checked_malloc(sizeof(vertex));
  vert->key = key;
  vert->x = x;
  vert->y = y; 
  return vert;
}

void InsertAll(problema prob, int linhas, int colunas, Heap *heap){
  vertex *I = NULL;
  for (int i = 0; i < linhas; i++){
    for (int j = 0; j < colunas; j++){
      if (prob.mapa[i][j] != 0){
        I = CreateVertex(i, j, INT_MAX / 2);
        HeapInsert(heap, I);
      }
    }   
  }
}

void Path_AtoB(int ***st, problema prob, int Xb, int Yb, int Xa, int Ya, solucao **S, int *count)
{
  static int idx;
  //coordenadas inacessiveis
  printf("TAS ONDE\n");
  if( Xb >= prob.nlinhas || Yb >= prob.ncolunas)
    exit(0);
  //não há solução
  if (st[Xb][Yb][0] == -1 || st[Xb][Yb][1] == -1)
    exit(0);
  //incrementa o numero de pontos do passeio
  (*count)++;

  //Para de chamar recursivamente quando chega ao segundo ponto do percurso 
  if ( (st[Xb][Yb][0] != Xa) || (st[Xb][Yb][1] != Ya) )
  {
    Path_AtoB(st, prob, st[Xb][Yb][0], st[Xb][Yb][1], Xa, Ya, S, count);
  }
  //já chegamos ao fim
  if((st[Xb][Yb][0] == Xa) && (st[Xb][Yb][1] == Ya)){
    idx = 0;
    (*S) = (solucao *) checked_malloc(sizeof(solucao));
    (*S)->n_pontos = *count; 
    (*S)->pontos = (int **)checked_malloc(sizeof(int *) * (*count));
    for(int i = 0; i < (*count); i++)
      (*S)->pontos[i] = (int*)checked_malloc(sizeof(int)*2);
  }

  //printf("Idx %d \n", idx);
  (*S)->pontos[idx][0] = st[Xb][Yb][0];
  (*S)->pontos[idx][1] = st[Xb][Yb][1];
  //imprime o ponto anterior
  printf("%d %d \n", (*S)->pontos[idx][0], (*S)->pontos[idx][1]);
  idx ++;
}

void GetAdjs(problema prob, int x, int y, int adj[][8] )
{
  // INICIALIZAR A -5 TUDO
  for (int i = 0; i < 8; i++)
  {
    adj[0][i] = -5;
    adj[1][i] = -5;
  }
  //verifica para cima
  if (x > 1)
  {
    if (y < prob.ncolunas - 1)
    { //verificar cima, direita
      if (prob.mapa[x - 2][y + 1] != 0)
      { //verifica se a celula que vai aceder é acessivel
        adj[0][Right2Up] = -2;
        adj[1][Right2Up] = 1;
      }
    }
    if (y > 0)
    { //verificar cima,esquerda
      if (prob.mapa[x - 2][y - 1] != 0)
      {
        adj[0][Left2Up] = -2;
        adj[1][Left2Up] = -1;
      }
    }

  }
  //verificar para baixo
  if (x < prob.nlinhas - 2)
  {
    if (y < prob.ncolunas - 1)
    { //verificar baixo, direita
      if (prob.mapa[x + 2][y + 1] != 0)
      {
        adj[0][Right2Down] = 2;
        adj[1][Right2Down] = 1;
      }
    }
    if (y > 0)
    { //verificar baixo, esquerda
      if (prob.mapa[x + 2][y - 1] != 0)
      {
        adj[0][Left2Down] = 2;
        adj[1][Left2Down] = -1;
      }
    }
  }
  //verificar para esquerda
  if (y > 1)
  {
    if (x < prob.nlinhas - 1)
    { //verificar esquerda, baixo
      if (prob.mapa[x + 1][y - 2] != 0)
      {
        adj[0][Down2Left] = 1;
        adj[1][Down2Left] = -2;
      }
    }
    if (x > 0)
    { //verificar esquerda, cima
      if (prob.mapa[x - 1][y - 2] != 0)
      {
        adj[0][Up2Left] = -1;
        adj[1][Up2Left] = -2;
      }
    }
  }
  //verificar para a direita
  if (y < prob.ncolunas - 2)
  {
    if (x < prob.nlinhas - 1)
    { //verificar direita, baixo
      if (prob.mapa[x + 1][y + 2] != 0)
      {
        adj[0][Down2Right] = 1;
        adj[1][Down2Right] = 2;
      }
    }
    if (x > 0)
    { //verificar direita, cima
      if (prob.mapa[x - 1][y + 2] != 0)
      {
        adj[0][Up2Right] = -1;
        adj[1][Up2Right] = 2;
      }
    }
  }
}

int FindIndex(Heap * h, int x, int y)
{
  vertex *V;
  for (int i = 0; i < getFree(h); i++)
  {
    V = getItem(h, i);
    if (V->x == x && V->y == y)
      return i;
  }
  exit(0);
}

int CompareKey(Item A, Item B)
{
  vertex *a = (vertex *)A, *b = (vertex *)B;
  return (a->key > b->key ? 1 : 0);
}

void printQueue(Heap * h)
{
  int i, size = getFree(h);
  vertex *V;
  for (i = 0; i < size; i++)
  {
    V = getItem(h, i);
    printf("%d\n", V->key);
  }
}