#include <limits.h>
#include <stdbool.h>

#include "utils.h"
#include "problemas.h"



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
      || prob.npontos <= 0 || (prob.modo == 'A' && prob.npontos != 2)) {
        ex_valido = 0;
  }
  return ex_valido;
}

//verifica se todos os pontos são acessiveis
int validate_points(problema prob){
  for (int i = 0; i < prob.npontos; i++) {
    //se tiver fora da matriz
    if ((prob.pontos[i][0] < 0) || (prob.pontos[i][0] >= prob.nlinhas) || (prob.pontos[i][1] >= prob.ncolunas))
      return 0;
    //se a casa for inacessivel
    if (prob.mapa[prob.pontos[i][0]][prob.pontos[i][1]] == 0)
      return 0;
    //se tiver rodeado de 0's  
    if (TrapedPoint(prob, prob.pontos[i][0], prob.pontos[i][1]))
      return 0;
  }
  return 1;
}

int TrapedPoint (problema P, int x, int y){
  int Adjs[8][2] = {{1, 2}, {-1, -2}, {-1, 2}, {1, -2}, {2, 1}, {-2, -1}, {2, -1}, {-2, 1}}; 
  int X, Y;
  for(int i = 0; i < 8; i++){
    if ( ( ( (X = Adjs[i][0] + x) >= 0) && (X < P.nlinhas) ) && (((Y = Adjs[i][1] + y) >= 0) && (Y < P.ncolunas))){
      if (P.mapa[X][Y] != 0) 
        return 0;
    }
  }
  return 1;     
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

solucao *solve_problem(FILE *fp, problema prob, int ***st, int **wt ){
  solucao *sol = NULL;

  if (validate_problem(prob) == 0 || validate_points(prob) == 0) {
    fprintf(fp, "%d %d %c %d %d %d\n", prob.nlinhas, prob.ncolunas, prob.modo, prob.npontos, -1, 0);
  }
  else if (prob.modo == 'A') {
     modoA(fp ,prob, st, wt); 
  }
  return sol;

}

//Encontra o melhor caminho entre A e B
//Retorna o numero de pontos do caminho
void DijkstraMagic(FILE *fp, problema prob, int **wt, int ***st,int Xa, int Ya, int Xb, int Yb){
  Heap *heap = NULL;
  vertex *V;
  int x, y, VertsAdjs[2][8], count = 0;
  solucao *sol;
  
  if ((Xa != Xb) || (Ya != Yb)){

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
    //Insere o ponto inicial 
    wt[Xa][Ya] = 0;
    HeapInsert(heap, CreateVertex(Xa, Ya, 0));

    //Enquanto o heap nao ta vazio e enquanto nao encontra a resposta #TODO
    while (EmptyHeap(heap) == 0)
    {
      V = getMostPri(heap);
      if (V->x == Xb && V->y == Yb)
        break;
      if (V->key != INT_MAX / 2)
      {
        InsertAndRelax_Adjs(heap, V, wt, st, prob);
      }
      HeapDeleteMostPri(heap);
    }
    free(getHeapData(heap));
    free(heap);
  }
  Path_AtoB(fp, st, wt, prob, Xb, Yb, Xa, Ya, &sol, &count);
  fprintf(fp, "\n");
}


void modoA (FILE *fp,problema prob, int ***st, int **wt){

  DijkstraMagic(fp, prob, wt, st, prob.pontos[0][0], prob.pontos[0][1], prob.pontos[1][0], prob.pontos[1][1]);
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

void Path_AtoB(FILE *fp, int ***st, int **wt, problema prob, int Xb, int Yb, int Xa, int Ya, solucao **S, int *count)
{
  int no_recursion = 0, sol;
  //não há solução
  if (st[Xb][Yb][0] == -1 || st[Xb][Yb][1] == -1){
    no_recursion = 1;
    sol = -1;
  }
  if (Xb == Xa && Ya == Yb){
   no_recursion = 1; 
   sol = 0;
  }

  if (no_recursion == 0)
  {
    (*count)++;

    //Para de chamar recursivamente quando chega ao segundo ponto do percurso
    if ((st[Xb][Yb][0] != Xa) || (st[Xb][Yb][1] != Ya))
    {
      Path_AtoB(fp, st, wt, prob, st[Xb][Yb][0], st[Xb][Yb][1], Xa, Ya, S, count);
    }
    if ((st[Xb][Yb][0] == Xa) && (st[Xb][Yb][1] == Ya))
    {
      fprintf(fp, "%d %d %c %d %d %d\n", prob.nlinhas, prob.ncolunas, prob.modo, prob.npontos, wt[prob.pontos[1][0]][prob.pontos[1][1]], *count);
    }
    //imprime o ponto anterior
    fprintf(fp, "%d %d %d\n", Xb, Yb, prob.mapa[Xb][Yb]);
  }
  else
  {
    fprintf(fp, "%d %d %c %d %d %d\n", prob.nlinhas, prob.ncolunas, prob.modo, prob.npontos, sol, 0);
  }
}

void InsertAndRelax_Adjs(Heap *h, vertex *V, int **wt, int ***st, problema prob)
{
  int x = V->x, y = V->y;

  //verifica para cima
  if (x > 1)
  {
    if (y < prob.ncolunas - 1)
    { //verificar cima, direita
      if (prob.mapa[x - 2][y + 1] != 0)
      { //verifica se a celula que vai aceder é acessivel
        //insere no heap se nunca teve no heap 
        if(wt[x-2][y+1] == INT_MAX/2){
          HeapInsert(h, CreateVertex(x - 2, y + 1, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, -2, 1);
      }
    }
    if (y > 0)
    { //verificar cima,esquerda
      if (prob.mapa[x - 2][y - 1] != 0)
      {
        //insere no heap se nunca teve no heap
        if (wt[x - 2][y - 1] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(x - 2, y - 1, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, -2, -1);
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
        //insere no heap se nunca teve no heap
        if (wt[x + 2][y + 1] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(x + 2, y + 1, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, 2, 1);
      }
    }
    if (y > 0)
    { //verificar baixo, esquerda
      if (prob.mapa[x + 2][y - 1] != 0)
      {
        //insere no heap se nunca teve no heap
        if (wt[x + 2][y - 1] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(x + 2, y - 1, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, 2, -1);
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
        //insere no heap se nunca teve no heap
        if (wt[x + 1][y - 2] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(x + 1, y - 2, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, 1, -2);
      }
    }
    if (x > 0)
    { //verificar esquerda, cima
      if (prob.mapa[x - 1][y - 2] != 0)
      {
        //insere no heap se nunca teve no heap
        if (wt[x - 1][y - 2] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(x - 1, y - 2, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, -1, -2);
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
        //insere no heap se nunca teve no heap
        if (wt[x + 1][y + 2] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(x + 1, y + 2, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, 1, 2);
      }
    }
    if (x > 0)
    { //verificar direita, cima
      if (prob.mapa[x - 1][y + 2] != 0)
      {
        //insere no heap se nunca teve no heap
        if (wt[x - 1][y + 2] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(x - 1, y + 2, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, prob, V, -1, 2);
      }
    }
  }
}

void RelaxEdge(Heap *heap, int **wt, int ***st, problema prob, vertex *V, int Xmove, int Ymove){
  //coordenadas do vértice adjacente
  int x = V->x + Xmove;
  int y = V->y + Ymove;
   
  //relaxa a aresta se encontra um caminho mais curto
  if (wt[x][y] > (V->key + prob.mapa[x][y]))
  {
    wt[x][y] = V->key + prob.mapa[x][y];
    //atualizar o st
    st[x][y][0] = V->x;
    st[x][y][1] = V->y;
    //o indice está correto
    ChangePri(heap, FindIndex(heap, x, y), CreateVertex(x, y, wt[x][y]));
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