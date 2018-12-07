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

  if ((prob.modo != 'A' && prob.modo != 'B') || prob.nlinhas <= 0 || prob.ncolunas <= 0 || prob.npontos <= 0 || 
      (prob.modo == 'A' && prob.npontos != 2) ||(prob.modo == 'B' && prob.npontos < 2))
  {
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
void free_problema(problema *estrutura, int ***st, int **wt, solucao *sol){
  int i, j;
  
  for(i = 0; i < (estrutura->npontos) - 1; i++){
    if(sol->passos[i] != NULL)
      free(sol->passos[i]); 
  }
  free(sol->n_passos);
  free(sol->passos);
  free(sol);

  for (i = 0; i < estrutura->nlinhas; i++) {
    for(j = 0; j < estrutura->ncolunas; j++)
      free(st[i][j]);
    free(st[i]);
    free(wt[i]);
    free(estrutura->mapa[i]);
  }
  free(st);
  free(wt);
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
    InitSolution(&sol, (prob.npontos)-1);
    sol->custo = -1;
    return sol;
  }
  else if (prob.modo == 'A') {
    sol = modoA(prob, st, wt); 
  }
  else if(prob.modo == 'B'){
    sol = modoB(prob, st, wt);
  }
  return sol;
}

//Encontra o melhor caminho entre A e B
//Retorna o numero de pontos do caminho
int DijkstraMagic(problema prob, int **wt, int ***st,int Xa, int Ya, int Xb, int Yb, solucao *sol, int *idx){
  Heap *heap = NULL;
  vertex *V; 
  
  //se o Ponto Inicial == Ponto Final nao é necessário aplicar o algoritmo
  if ((Xa == Xb) && (Ya == Yb)){
    (*idx)++;
    return 1;
  }
  //inicializa os vetores auxiliares
  for (int v = 0; v < prob.nlinhas; v++)
  {
    for (int w = 0; w < prob.ncolunas; w++)
    {
      //[v][w][0] = x  || [v][w][1] = y
      st[v][w][0] = -1;
      st[v][w][1] = -1;
      wt[v][w] = INT_MAX / 2;
    }
  }
  //inicializa o Heap
  heap = HeapInit(CompareKey, SentCoordinates, prob.nlinhas, prob.ncolunas);
  //Insere o ponto inicial
  wt[Xa][Ya] = 0;
  HeapInsert(heap, CreateVertex(Xa, Ya, 0));

  //Enquanto não foi a todos os vértices
  while (EmptyHeap(heap) == 0)
  {
    V = getMostPri(heap);
    if (V->x == Xb && V->y == Yb) //se já achou o caminho mais curto para o vértice pretendido
      break;
    if (V->key != INT_MAX / 2)
    {
      InsertAndRelax_Adjs(heap, V, wt, st, prob);
    }
    HeapDeleteMostPri(heap);
  }
  freeHeap(heap);
  sol->custo += wt[Xb][Yb];
  Path_AtoB(st, wt, prob, Xb, Yb, Xa, Ya, sol, *idx);
  if(sol->custo == -1) return 1; //nao necessita de repetir a função (modoB)
  (*idx)++;
  return 0;
}

solucao *modoB (problema prob, int ***st, int **wt){
  int idx = 0 , stop = 0;
  solucao *sol;
  InitSolution(&sol, (prob.npontos) - 1);
  for (int i = 0; i < prob.npontos - 1; i++)
  {
    stop = DijkstraMagic(prob, wt, st, prob.pontos[i][0], prob.pontos[i][1], prob.pontos[i+1][0], prob.pontos[i+1][1], sol, &idx);
    if(stop) break;
  }
  return sol;
}

solucao *modoA (problema prob, int ***st, int **wt){
  solucao *sol = NULL;
  int idx = 0;
  InitSolution(&sol, (prob.npontos)-1);
  DijkstraMagic(prob, wt, st, prob.pontos[0][0], prob.pontos[0][1], prob.pontos[1][0], prob.pontos[1][1], sol, &idx);
  return sol;
}

Item CreateVertex(int x, int y, int key){ 
  vertex * vert = (vertex*)checked_malloc(sizeof(vertex));
  vert->key = key;
  vert->x = x;
  vert->y = y; 
  return vert;
}


void Path_AtoB(int ***st, int **wt, problema prob, int Xb, int Yb, int Xa, int Ya, solucao *S, int idx)
{
  static int count = 0;
  static int aux = 0;

  //se nao há solução
  if (st[Xb][Yb][0] == -1 || st[Xb][Yb][1] == -1){
    S->custo = -1;
    return;
  }
  //Incrementa o número de pontos do caminho
  count++;
  //Para de chamar recursivamente quando chega ao segundo ponto do percurso
  if ((st[Xb][Yb][0] != Xa) || (st[Xb][Yb][1] != Ya))
  {      
    Path_AtoB(st, wt, prob, st[Xb][Yb][0], st[Xb][Yb][1], Xa, Ya, S, idx);
  }
  if ((st[Xb][Yb][0] == Xa) && (st[Xb][Yb][1] == Ya))
  {
    aux = 0;
    S->passos[idx] = (vertex *)checked_malloc(sizeof(vertex) * count);
    S->n_passos[idx] = count;
  }
  S->passos[idx][aux].x = Xb;
  S->passos[idx][aux].y = Yb;
  S->passos[idx][aux].key = prob.mapa[Xb][Yb];
  aux++;
  //reinicia o contador
  count = 0;
}

void InsertAndRelax_Adjs(Heap *h, vertex *V, int **wt, int ***st, problema P)
{
  int x = V->x, y = V->y, X, Y;
  int Adjs[8][2] = {{1, 2}, {-1, -2}, {-1, 2}, {1, -2}, {2, 1}, {-2, -1}, {2, -1}, {-2, 1}};

  for (int i = 0; i < 8; i++){
    if ((((X = Adjs[i][0] + x) >= 0) && (X < P.nlinhas)) && (((Y = Adjs[i][1] + y) >= 0) && (Y < P.ncolunas))){
      if (P.mapa[X][Y] != 0){
        //insere no heap se nunca teve no heap
        if (wt[X][Y] == INT_MAX / 2)
        {
          HeapInsert(h, CreateVertex(X, Y, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, P, V, Adjs[i][0], Adjs[i][1]);
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
    ChangePri(heap, CreateVertex(x, y, wt[x][y]));
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

void print_sol(FILE *fp, problema *p, solucao *Sol)
{
  int n_passos = 0;

  if(Sol->custo == -1){
    fprintf(fp, "%d %d %c %d %d %d\n", p->nlinhas, p->ncolunas, p->modo, p->npontos, -1, 0);
  }
  else {
    for (int i = 0; i < ((p->npontos) - 1); i++){
      n_passos += Sol->n_passos[i]; 
    }
    fprintf(fp, "%d %d %c %d %d %d\n", p->nlinhas, p->ncolunas, p->modo, p->npontos, Sol->custo, n_passos);
    for (int i = 0; i < ((p->npontos) - 1); i++)
    {
      for (int j = 0; j < Sol->n_passos[i]; j++)
      {
        fprintf(fp, "%d %d %d\n", Sol->passos[i][j].x, Sol->passos[i][j].y, Sol->passos[i][j].key);
      }
    }
  }
  fprintf(fp, "\n");
}

int SentCoordinates(Item a, int NCols){
  vertex *A = a;
  return (A->x * NCols + A->y);
}

void InitSolution(solucao **S, int size){
  (*S) = (solucao *)checked_malloc(sizeof(solucao));
  (*S)->n_passos = (int *)checked_malloc(sizeof(int) * size);
  (*S)->passos = (vertex **)checked_malloc(sizeof(vertex *) * size);
  for(int i = 0; i < size; i++){
    (*S)->passos[i] = NULL;
    (*S)->n_passos[i] = 0;
  }
  (*S)->custo = 0;
  (*S)->valido = 0;
}


