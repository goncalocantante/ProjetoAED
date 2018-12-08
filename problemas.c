#include <limits.h>

#include "utils.h"
#include "problemas.h"

problema *ler_problema(FILE *fp, vertex ***st, int ***wt, solucao **sol)
{
  int i = 0, j = 0, n_scan = 0, discard = 0, discard1 = 0;
  problema *prob = NULL;

  prob = (problema *)checked_malloc(sizeof(problema));
  //retira os dados da primeira linha do ficheiro
  n_scan = fscanf (fp,"%d %d %c %d", &prob->nlinhas , &prob->ncolunas, &prob->modo, &prob->npontos);
  if (n_scan == EOF) {    //indica se há outro problema ou não 
    free(prob);
    return NULL;
  }
  
  //só continua se o problema for válido
  if (validate_problem(*prob) == 0)
  {
    for (i = 0; i < prob->npontos; i++){
      n_scan = fscanf(fp, "%d %d", &discard, &discard1);
    }
    for (i = 0; i < prob->nlinhas; i++){
      for (int j = 0; j < prob->ncolunas; j++){
        n_scan = fscanf(fp, "%d", &discard);
      }
    }
    InitSolution(sol, ((prob->npontos) - 1));
    (*sol)->custo = -1;
  }else{
    //guarda coordenadas dos pontos numa matriz
    prob->pontos = (vertex*)checked_malloc(sizeof(vertex)*prob->npontos);
    for (i = 0; i < prob->npontos; i++){
        n_scan = fscanf(fp, "%d %d", &prob->pontos[i].x, &prob->pontos[i].y);
    }
    //matriz do mapa da cidade
    prob->mapa = (int**)checked_malloc(sizeof(int*)*prob->nlinhas);
    //vetores auxiliares para algoritmo de Dijkstra
    (*wt) = (int **)checked_malloc(sizeof(int *) * prob->nlinhas);
    (*st) = (vertex**)checked_malloc(sizeof(vertex*) * prob->nlinhas);
    for (i = 0; i < prob->nlinhas; i++){
      prob->mapa[i] = (int*)checked_malloc(sizeof(int)*prob->ncolunas);
      (*wt)[i] = (int *)checked_malloc(sizeof(int) * prob->ncolunas);
      (*st)[i] = (vertex *)checked_malloc(sizeof(vertex) * prob->ncolunas);
      for (j = 0; j < prob->ncolunas; j++){
        n_scan = fscanf(fp,"%d", &prob->mapa[i][j]);
      
      }
    } 
  }
  return prob;
}

int validate_problem(problema prob){
  int ex_valido = 1;

  if ((prob.modo != 'A' && prob.modo != 'B' && prob.modo != 'C') || prob.nlinhas <= 0 || prob.ncolunas <= 0 || prob.npontos <= 0 ||
      (prob.modo == 'A' && prob.npontos != 2) || (prob.modo == 'B' && prob.npontos < 2) || (prob.modo == 'C' && prob.npontos < 2))
  {
    ex_valido = 0;
  }
  return ex_valido;
}

//verifica se todos os pontos são acessiveis
int validate_points(problema prob){
  for (int i = 0; i < prob.npontos; i++) {
    //se tiver fora da matriz
    if ((prob.pontos[i].x < 0) || (prob.pontos[i].x >= prob.nlinhas) || (prob.pontos[i].y < 0) || (prob.pontos[i].y >= prob.ncolunas))
      return 0;
    //se a casa for inacessivel
    if (prob.mapa[prob.pontos[i].x][prob.pontos[i].y] == 0)
      return 0;
    //se tiver rodeado de 0's  
    if (TrapedPoint(prob, prob.pontos[i].x, prob.pontos[i].y))
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
void free_problema(problema *estrutura, vertex **st, int **wt, solucao *sol){
  int i;
  for(i = 0; i < (estrutura->npontos) - 1; i++){
    if(sol->passos[i] != NULL)
        free(sol->passos[i]); 
  }
  free(sol->n_passos);
  free(sol->passos);
  free(sol);
  
  if (st != NULL){
    for (i = 0; i < estrutura->nlinhas; i++) {
        free(st[i]);
        free(wt[i]);
        free(estrutura->mapa[i]);
      }
    free(st);
    free(wt);
    free(estrutura->mapa);
    free(estrutura->pontos);
  }
  free(estrutura);
}

solucao *solve_problem(problema prob, vertex **st, int **wt ){
  solucao *sol = NULL;
  if ( validate_points(prob) == 0) {
    InitSolution(&sol, (prob.npontos)-1);
    sol->custo = -1;
    return sol;
  } 
  if (prob.modo == 'A') {
    sol = modoA(prob, st, wt); 
  }
  else if(prob.modo == 'B'){
    sol = modoB(prob, st, wt);
  }
  else if(prob.modo == 'C'){
    sol = modoC(prob, st, wt);
  }
  return sol;
}

//Encontra o melhor caminho entre A e B
//Retorna o numero de pontos do caminho
int DijkstraMagic(problema prob, int **wt, vertex **st,int Xa, int Ya, int Xb, int Yb){
  Heap *heap = NULL;
  HeapNode *V; 

  //inicializa os vetores auxiliares
  for (int v = 0; v < prob.nlinhas; v++)
  {
    for (int w = 0; w < prob.ncolunas; w++)
    {
      //[v][w][0] = x  || [v][w][1] = y
      st[v][w].x = -1;
      st[v][w].y = -1;
      wt[v][w] = INT_MAX / 2;
    }
  }
  //inicializa o Heap
  heap = HeapInit(CompareKey, SentCoordinates, prob.nlinhas, prob.ncolunas);
  //Insere o ponto inicial
  wt[Xa][Ya] = 0;
  HeapInsert(heap, CreateHeapNode(Xa, Ya, 0));

  //Enquanto não foi a todos os vértices
  while (EmptyHeap(heap) == 0)
  {
    V = getMostPri(heap);
    if (V->coord.x == Xb && V->coord.y == Yb) //se já achou o caminho mais curto para o vértice pretendido
      break;
    if (V->key != INT_MAX / 2)
    {
      InsertAndRelax_Adjs(heap, V, wt, st, prob);
    }
    HeapDeleteMostPri(heap);
  }
  freeHeap(heap);
  
  return 0;
}


solucao *modoC (problema prob, vertex **st, int **wt){
  solucao *sol;
  int stop = 0, idx = 0;
  int **matrix = NULL;
  
  InitSolution(&sol, (prob.npontos) - 1);
  matrix = (int **)checked_malloc(sizeof(int *) * prob.npontos);  

  for (int i = 0; i < prob.npontos; i++){
    matrix[i] = (int*)checked_malloc(sizeof(int) * prob.npontos);
    for (int j = i + 1; j < prob.npontos - 1; j++)
    {
      //se o Ponto Inicial == Ponto Final nao é necessário aplicar o algoritmo
      if ((prob.pontos[j].x != prob.pontos[j + 1].x) || (prob.pontos[j].y != prob.pontos[j + 1].y)){
        DijkstraMagic(prob, wt, st, prob.pontos[i].x, prob.pontos[i].y, prob.pontos[j].x, prob.pontos[j].y);   
        sol->custo += wt[prob.pontos[i+1].x][prob.pontos[i+1].y];
        Path_AtoB(st, wt, prob, prob.pontos[i+1].x, prob.pontos[i+1].y, prob.pontos[i].x, prob.pontos[i].y, sol, idx);        
        printf("%d\n", sol->custo);
      }
      matrix[i][j] = sol->custo;      
      if(sol->custo == -1) break; //nao necessita de repetir a função (modoC)     
    }
    //não tenho a certeza se o break quebra os dois fors então fica aqui o outro por segurança
    if(sol->custo == -1) break; //nao necessita de repetir a função (modoC)
  }

 /*  for(int i = 0; i < prob.npontos; i++){
    for(int j = 0; j < prob.npontos; j++){
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  } */
  return sol;
}

solucao *modoB (problema prob, vertex **st, int **wt){
  int idx = 0 , stop = 0;
  solucao *sol;
  InitSolution(&sol, (prob.npontos) - 1);
  
  for (int i = 0; i < prob.npontos - 1; i++)
  {
    //se o Ponto Inicial == Ponto Final nao é necessário aplicar o algoritmo
     if ((prob.pontos[i].x == prob.pontos[i + 1].x) && (prob.pontos[i].y == prob.pontos[i + 1].y)){
      idx++;
      i++;
     }
    DijkstraMagic(prob, wt, st, prob.pontos[i].x, prob.pontos[i].y, prob.pontos[i+1].x, prob.pontos[i+1].y);
    sol->custo += wt[prob.pontos[i+1].x][prob.pontos[i+1].y];
    Path_AtoB(st, wt, prob, prob.pontos[i+1].x, prob.pontos[i+1].y, prob.pontos[i].x, prob.pontos[i].y, sol, idx);
    if(sol->custo == -1) break; //nao necessita de repetir a função (modoB)
    idx++;
  }
  return sol;
}

 solucao *modoA (problema prob, vertex **st, int **wt){
  solucao *sol = NULL;
  int idx = 0;
  InitSolution(&sol, (prob.npontos)-1);
  //se o Ponto Inicial == Ponto Final nao é necessário aplicar o algoritmo
  if ((prob.pontos[0].x != prob.pontos[1].x) || (prob.pontos[0].y != prob.pontos[1].y)){
    DijkstraMagic(prob, wt, st, prob.pontos[0].x, prob.pontos[0].y, prob.pontos[1].x, prob.pontos[1].y);
    sol->custo += wt[prob.pontos[1].x][prob.pontos[1].y];
    Path_AtoB(st, wt, prob, prob.pontos[1].x, prob.pontos[1].y, prob.pontos[0].x, prob.pontos[0].y, sol, idx);
  }
  return sol;
} 

Item CreateHeapNode(int x, int y, int key){
  HeapNode *vert = (HeapNode *)checked_malloc(sizeof(HeapNode));
  vert->key = key;
  vert->coord.x = x;
  vert->coord.y = y; 
  return vert;
}

void Path_AtoB(vertex **st, int **wt, problema prob, int Xb, int Yb, int Xa, int Ya, solucao *S, int idx)
{
  static int count = 0;
  static int aux = 0;

  //se nao há solução
  if (st[Xb][Yb].x == -1 || st[Xb][Yb].y == -1){
    S->custo = -1;
    return;
  }
  //Incrementa o número de pontos do caminho
  count++;
  //Para de chamar recursivamente quando chega ao segundo ponto do percurso
  if ((st[Xb][Yb].x != Xa) || (st[Xb][Yb].y != Ya))
  {
    Path_AtoB(st, wt, prob, st[Xb][Yb].x, st[Xb][Yb].y, Xa, Ya, S, idx);
  }
  if ((st[Xb][Yb].x == Xa) && (st[Xb][Yb].y == Ya))
  {
    aux = 0;
    S->passos[idx] = (vertex *)checked_malloc(sizeof(vertex)*count) ;
    S->n_passos[idx] = count;
  }
  S->passos[idx][aux].x = Xb;
  S->passos[idx][aux].y = Yb;
  aux++;
  //reinicia o contador
  count = 0;
}

void InsertAndRelax_Adjs(Heap *h, HeapNode *V, int **wt, vertex **st, problema P)
{
  int x = V->coord.x, y = V->coord.y, X, Y;
  int Adjs[8][2] = {{1, 2}, {-1, -2}, {-1, 2}, {1, -2}, {2, 1}, {-2, -1}, {2, -1}, {-2, 1}};

  for (int i = 0; i < 8; i++){
    if ((((X = Adjs[i][0] + x) >= 0) && (X < P.nlinhas)) && (((Y = Adjs[i][1] + y) >= 0) && (Y < P.ncolunas))){
      if (P.mapa[X][Y] != 0){
        //insere no heap se nunca teve no heap
        if (wt[X][Y] == INT_MAX / 2)
        {
          HeapInsert(h, CreateHeapNode(X, Y, INT_MAX / 2));
        }
        RelaxEdge(h, wt, st, P, V, Adjs[i][0], Adjs[i][1]);
      }
    }
  }
}

void RelaxEdge(Heap *heap, int **wt, vertex **st, problema prob, HeapNode *V, int Xmove, int Ymove){
  //coordenadas do vértice adjacente
  int x = V->coord.x + Xmove;
  int y = V->coord.y + Ymove;

  //relaxa a aresta se encontra um caminho mais curto
  if (wt[x][y] > (V->key + prob.mapa[x][y]))
  {
    wt[x][y] = V->key + prob.mapa[x][y];
    //atualizar o st
    st[x][y].x = V->coord.x;
    st[x][y].y = V->coord.y;
    //o indice está correto
    ChangePri(heap, CreateHeapNode(x, y, wt[x][y]));
  }
}


int CompareKey(Item A, Item B)
{
  HeapNode *a = (HeapNode *)A, *b = (HeapNode *)B;
  return (a->key > b->key ? 1 : 0);
}

void printQueue(Heap * h)
{
  int i, size = getFree(h);
  HeapNode *V;
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
        fprintf(fp, "%d %d %d\n", Sol->passos[i][j].x, Sol->passos[i][j].y, p->mapa[Sol->passos[i][j].x][Sol->passos[i][j].y]);
      }
    }
  }
  fprintf(fp, "\n");
}

int SentCoordinates(Item a, int NCols){
  HeapNode *A = a;
  return (A->coord.x * NCols + A->coord.y);
}

void InitSolution(solucao **S, int size){
  if (size < 0) size = 0;
  (*S) = (solucao *)checked_malloc(sizeof(solucao));
  (*S)->n_passos = (int *)checked_malloc(sizeof(int) * size);
  (*S)->passos = (vertex **)checked_malloc(sizeof(vertex*) * size);
  for(int i = 0; i < size; i++){
    (*S)->passos[i] = NULL;
    (*S)->n_passos[i] = 0;
  }
  (*S)->custo = 0;
  (*S)->valido = 0;
}


