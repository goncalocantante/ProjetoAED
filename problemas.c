#include <limits.h>

#include "utils.h"
#include "problemas.h"

Problema *ler_problema(FILE *fp, Vertex ***st, int ***wt, Passeio **passeio)
{
  int i = 0, j = 0, n_scan = 0, discard = 0, discard1 = 0;
  Problema *prob = NULL;

  prob = (Problema *)checked_malloc(sizeof(Problema));
  n_scan = fscanf (fp,"%d %d %c %d", &prob->nlinhas , &prob->ncolunas, &prob->modo, &prob->npontos);
  //retira os dados da primeira linha do ficheiro
  if (n_scan == EOF) {    //indica se há outro Problema ou não  
    free(prob);
    return NULL;
  }
  //só continua se o Problema for válido
  if (validate_problem(*prob) == 0)
  {
    for (i = 0; i < prob->npontos; i++){
      n_scan = fscanf(fp, "%d %d", &discard, &discard1);
    }
    for (i = 0; i < prob->nlinhas; i++){               //#TODO tentar fazer sem for for somar os limites 
      for (int j = 0; j < prob->ncolunas; j++){
        n_scan = fscanf(fp, "%d", &discard);
      }
    }
    InitPasseio(passeio, 0);
    (*passeio)->CustoTotal = -1;
  }else{
    //guarda coordenadas dos pontos numa matriz
    prob->pontos = (Vertex*)checked_malloc(sizeof(Vertex)*prob->npontos);
    for (i = 0; i < prob->npontos; i++){
        n_scan = fscanf(fp, "%d %d", &prob->pontos[i].x, &prob->pontos[i].y);
    }
    //matriz do mapa da cidade
    prob->mapa = (int**)checked_malloc(sizeof(int*)*prob->nlinhas);
    //vetores auxiliares para algoritmo de Dijkstra
    (*wt) = (int **)checked_malloc(sizeof(int *) * prob->nlinhas);
    (*st) = (Vertex**)checked_malloc(sizeof(Vertex*) * prob->nlinhas);
    for (i = 0; i < prob->nlinhas; i++){
      prob->mapa[i] = (int*)checked_malloc(sizeof(int)*prob->ncolunas);
      (*wt)[i] = (int *)checked_malloc(sizeof(int) * prob->ncolunas);
      (*st)[i] = (Vertex *)checked_malloc(sizeof(Vertex) * prob->ncolunas);
      for (j = 0; j < prob->ncolunas; j++){
        n_scan = fscanf(fp,"%d", &prob->mapa[i][j]);
      }
    } 
  }
  return prob;
}

int validate_problem(Problema prob){
  int ex_valido = 1;

  if ((prob.modo != 'A' && prob.modo != 'B' && prob.modo != 'C') || prob.nlinhas <= 0 || prob.ncolunas <= 0 || prob.npontos <= 0 ||
      (prob.modo == 'A' && prob.npontos != 2) || (prob.modo == 'B' && prob.npontos < 2) || (prob.modo == 'C' && prob.npontos < 2))
  {
    ex_valido = 0;
  }
  return ex_valido;
}

//verifica se todos os pontos são acessiveis
int validate_points(Problema prob){
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

int TrapedPoint (Problema P, int x, int y){
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
void free_problema(Problema *estrutura, Vertex **st, int **wt, Passeio *passeio){
  int i;
  
  if(passeio->passos != NULL){
    for(i = 0; i < passeio->n_passos; i++){
      if(passeio->passos[i] != NULL){
        if (passeio->passos[i]->passos != NULL){
          free(passeio->passos[i]->passos);
        } 
        free(passeio->passos[i]);
      }
    }
    free(passeio->passos);
  }
  free(passeio);
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

Passeio *solve_problem(Problema prob, Vertex **st, int **wt ){
  Passeio *passeio = NULL;
  if ( validate_points(prob) == 0) {
    InitPasseio(&passeio, (prob.npontos)-1);
    passeio->CustoTotal = -1;
    return passeio;
  } 
  if (prob.modo == 'A') {
    InitPasseio(&passeio, 1);
    passeio->passos[0] = modoA(prob, st, wt); 
    passeio->CustoTotal = passeio->passos[0]->custo;
  }
   else if(prob.modo == 'B'){
    passeio = modoB(prob, st, wt);
  } 
  else if(prob.modo == 'C'){
    passeio = modoC(prob, st, wt);
  }
  return passeio;
}

//Encontra o melhor caminho entre A e B
//Retorna o numero de pontos do caminho
void DijkstraMagic(Problema prob, int **wt, Vertex **st,int Xa, int Ya, HeapNode *stop, int size){
  Heap *heap = NULL;
  HeapNode *V;

  //inicializa os vetores auxiliares
  for (int v = 0; v < prob.nlinhas; v++)
  {
    for (int w = 0; w < prob.ncolunas; w++)
    {
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
  //Verificar se todos os pontos do caminho já foram verificados (Parte C)
  if (StopDijkstra(stop, size, (V->coord.x), (V->coord.y) ) ) break;

  /* if (V->coord.x == Xb && V->coord.y == Yb) 
    break;                                          |||Em vez disto o que tá em cima||| */

  if (V->key != INT_MAX / 2)
  {
    InsertAndRelax_Adjs(heap, V, wt, st, prob);
  }
  HeapDeleteMostPri(heap);
  }
  freeHeap(heap);                         
}


Passeio *modoC(Problema prob, Vertex **st, int **wt)
{
  int size, stop = 0, j, *vert = (int *)checked_malloc(sizeof(int) * (prob.npontos - 1));
  HeapNode *vect;
  Passo ***matrix = NULL;
  Passeio *passeio = NULL;

  matrix = (Passo ***)checked_malloc(sizeof(Passo **) * prob.npontos);
  for (int i = 0; i < prob.npontos; i++) //aloca memória para a matriz de adjacências
    matrix[i] = (Passo **)checked_malloc(sizeof(Passo *) * prob.npontos);

  //preeenche a matriz de adjacencias
  for (int i = 0; i < prob.npontos; i++){
    InitPasso(&(matrix[i][i]), 0);
    size = (prob.npontos - i - 1);
    InitVect(prob, &vect, size, i);

    //if ((prob.pontos[j].x != prob.pontos[j + 1].x) || (prob.pontos[j].y != prob.pontos[j + 1].y)){
    DijkstraMagic(prob, wt, st, prob.pontos[i].x, prob.pontos[i].y, vect, size);

    //guarda o caminho
    for (j = i + 1; j < prob.npontos; j++){
      Path_AtoB(st, wt, prob, prob.pontos[j].x, prob.pontos[j].y, prob.pontos[i].x, prob.pontos[i].y, &(matrix[i][j]));
      if (matrix[i][j]->custo == -1){
        stop = 1;
        break;
      }
      matrix[i][j]->custo = wt[prob.pontos[j].x][prob.pontos[j].y];
      //calcula posição transposta
      //InitPasso(&(matrix[j][i]), matrix[i][j]->n_passos);  
      Path_AtoB(st, wt, prob, prob.pontos[i].x, prob.pontos[i].y, prob.pontos[j].x, prob.pontos[j].y, &(matrix[j][i]));
      matrix[j][i]->custo = wt[prob.pontos[i].x][prob.pontos[i].y];
    }
    free(vect);    
    if (stop){
      InitPasseio(&passeio, 0);
      passeio->CustoTotal = -1;
      return passeio;
    }
  }

  for(int i = 0; i < prob.npontos; i++){
    for (int j = 0; j < prob.npontos; j++){
      printf(" %d ", matrix[i][j]->custo);
    } 
    printf("\n");  
  }
  InitPasseio(&passeio, (prob.npontos - 1));
  passeio->CustoTotal = INT_MAX / 2;
  for (int j = 1; j < prob.npontos; j++)
  {
    vert[j - 1] = j; //atribuir a cada ponto 1 número inteiro
  }

  PermutationBeast(vert, 0, prob.npontos - 1, matrix, passeio);

  return passeio; //este return ta mal mas era so para nao dar erro
}

Passeio *modoB (Problema prob, Vertex **st, int **wt){
  int idx = 0;
  Passeio *passeio;
  HeapNode V;

  InitPasseio(&passeio, (prob.npontos) - 1);
  for (int i = 0; i < prob.npontos - 1; i++)
  {
     //se o Ponto Inicial == Ponto Final nao é necessário aplicar o algoritmo
    if ((prob.pontos[i].x == prob.pontos[i + 1].x) && (prob.pontos[i].y == prob.pontos[i + 1].y)){
      InitPasso(&(passeio->passos[idx]), 0);
    }else{
      V.coord.x = prob.pontos[i+1].x;
      V.coord.y = prob.pontos[i+1].y;
      V.key = 1;
      DijkstraMagic(prob, wt, st, prob.pontos[i].x, prob.pontos[i].y, &V, 1);
      Path_AtoB(st, wt, prob, prob.pontos[i+1].x, prob.pontos[i+1].y, prob.pontos[i].x, prob.pontos[i].y, &(passeio->passos[idx]));
      if(passeio->passos[idx]->custo == -1){
        passeio->CustoTotal = -1;
        break; //nao necessita de repetir a função (modoB)
      }
      passeio->passos[idx]->custo = wt[prob.pontos[i + 1].x][prob.pontos[i + 1].y];
    }
    passeio->CustoTotal += passeio->passos[idx]->custo;
    idx++;
  }
  return passeio;
} 

Passo *modoA (Problema prob, Vertex **st, int **wt){
  Passo *passo;

  HeapNode *V = (HeapNode *)checked_malloc(sizeof(HeapNode));
  V->coord.x = prob.pontos[1].x;
  V->coord.y = prob.pontos[1].y;
  V->key = 1;

  //se o Ponto Inicial == Ponto Final nao é necessário aplicar o algoritmo
  if ((prob.pontos[0].x != prob.pontos[1].x) || (prob.pontos[0].y != prob.pontos[1].y)){
    DijkstraMagic(prob, wt, st, prob.pontos[0].x, prob.pontos[0].y, V, 1);
    Path_AtoB(st, wt, prob, prob.pontos[1].x, prob.pontos[1].y, prob.pontos[0].x, prob.pontos[0].y, &passo);
    if (passo->custo != -1)
      passo->custo += wt[prob.pontos[1].x][prob.pontos[1].y];

  }else{
    InitPasso(&passo, 0);
  }
  free(V);
  return passo;
} 

Item CreateHeapNode(int x, int y, int key){
  HeapNode *vert = (HeapNode *)checked_malloc(sizeof(HeapNode));
  vert->key = key;
  vert->coord.x = x;
  vert->coord.y = y; 
  return vert;
}

void Path_AtoB(Vertex **st, int **wt, Problema prob, int Xb, int Yb, int Xa, int Ya, Passo **passo)
{
  static int count = 0;
  static int aux = 0;

  //se nao há solução
  if (st[Xb][Yb].x == -1 || st[Xb][Yb].y == -1){
    InitPasso(passo, 0);
    (*passo)->custo = -1;
    return;
  }
  //Incrementa o número de pontos do caminho
  count++;
  //Para de chamar recursivamente quando chega ao segundo ponto do percurso
  if ((st[Xb][Yb].x != Xa) || (st[Xb][Yb].y != Ya)){
    Path_AtoB(st, wt, prob, st[Xb][Yb].x, st[Xb][Yb].y, Xa, Ya, passo);
  }
  
  if ((st[Xb][Yb].x == Xa) && (st[Xb][Yb].y == Ya))
  {
    aux = 0;
    InitPasso(passo, count);
  }
  (*passo)->passos[aux].x = Xb;
  (*passo)->passos[aux].y = Yb;
  aux++;
  //reinicia o contador
  count = 0;
}

void InsertAndRelax_Adjs(Heap *h, HeapNode *V, int **wt, Vertex **st, Problema P)
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

void RelaxEdge(Heap *heap, int **wt, Vertex **st, Problema prob, HeapNode *V, int Xmove, int Ymove){
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

void print_sol(FILE *fp, Problema *p, Passeio *passeio)
{
  int n_passos = 0;

  
  if(passeio->CustoTotal == -1){
    fprintf(fp, "%d %d %c %d %d %d\n", p->nlinhas, p->ncolunas, p->modo, p->npontos, -1, 0);
  }
  else {
    for (int i = 0; i < passeio->n_passos; i++){
      if (passeio->passos[i] != NULL)
        n_passos += passeio->passos[i]->n_passos; 
    }
    fprintf(fp, "%d %d %c %d %d %d\n", p->nlinhas, p->ncolunas, p->modo, p->npontos, passeio->CustoTotal, n_passos);
    printf("passeio n passos = %d\n", passeio->n_passos);
    for (int i = 0; i < passeio->n_passos; i++){
      if (passeio->passos[i] != NULL){
        for (int j = 0; j < passeio->passos[i]->n_passos; j++)
        {
            fprintf(fp, "%d %d %d\n", passeio->passos[i]->passos[j].x, passeio->passos[i]->passos[j].y, p->mapa[passeio->passos[i]->passos[j].x][passeio->passos[i]->passos[j].y]);
        }
      }
    }
  }
  fprintf(fp, "\n");
}

int SentCoordinates(Item a, int NCols){
  HeapNode *A = a;
  return (A->coord.x * NCols + A->coord.y);
}

void InitPasseio(Passeio **P, int size){
  if (size < 0) size = 0;
  (*P) = (Passeio*)checked_malloc(sizeof(Passeio));
  (*P)->CustoTotal = 0;
  (*P)->n_passos = size;
  if (size > 0){
   (*P)->passos = (Passo**)checked_malloc(sizeof(Passo*) * size);
   for (int i = 0; i < size; i++)
     (*P)->passos[i] = NULL;
  }else
   (*P)->passos = NULL;
}

void InitPasso (Passo **passo, int n_passos){
  if (n_passos < 0) n_passos = 0;
  (*passo) = (Passo*)checked_malloc(sizeof(Passo));
  (*passo)->custo = 0;
  (*passo)->n_passos = n_passos;
  if (n_passos > 0){
   (*passo)->passos = (Vertex*)checked_malloc(sizeof(Vertex)*n_passos);
  }else
    (*passo)->passos = NULL;
}

void PermutationBeast(int *array, int i, int length, Passo ***matrix, Passeio *best_passeio)
{
  int aux;
  if (length == i)
  {
    printArr(array, length, matrix, best_passeio); /* Aqui termina uma iteração */ //vai tar aqui uma funçao que calcula o peso do caminho para aquela iteração
    return;
  }
  int j = i;
  for (j = i; j < length; j++)
  {
    aux = array[i];
    array[i] = array[j];
    array[j] = aux;
    PermutationBeast(array, i + 1, length, matrix, best_passeio);
    aux = array[i];
    array[i] = array[j];
    array[j] = aux;
  }
  return;
}

//Prints the array
void printArr(int *a, int n, Passo ***matrix, Passeio *passeio_min)
{
  Passeio *passeio_atual;
  InitPasseio(&passeio_atual, n);

  passeio_atual->passos[0] = matrix[0][a[0]];
  passeio_atual->CustoTotal += matrix[0][a[0]]->custo;

  for (int i = 1; i < n ; i++)
  {
    //este meu vetor passa a apontar para a posição da matriz
    passeio_atual->passos[i] = matrix[a[i-1]][a[i]]; //custo

    passeio_atual->CustoTotal += matrix[a[i-1]][a[i]]->custo;

  }

  if (passeio_atual->CustoTotal < passeio_min->CustoTotal){
    passeio_min->CustoTotal = passeio_atual->CustoTotal;
    passeio_min->n_passos = passeio_atual->n_passos; //nao está inicializado
    free(passeio_min->passos);
    passeio_min->passos = passeio_atual->passos;
  }

  //free(passeio_atual->passos);
  free(passeio_atual);
}

int StopDijkstra(HeapNode *stop, int size, int x, int y){   
  for(int i = 0; i < size; i++){
    if(stop[i].coord.x == x  && stop[i].coord.y == y)
      stop[i].key = 0;
    if (stop[i].key == 1) return 0;   //se econtra algum ponto que nao foi visto
  }
  return 1;
}

void InitVect(Problema prob, HeapNode ** vect, int size, int i){
  (*vect) = (HeapNode *)checked_malloc(sizeof(HeapNode) * size);
  //inicializa o vetor de pontos a que o Dijkstra tem de ir
  for (int j = 0; j < size; j++)
  {
    (*vect)[j].coord.x = prob.pontos[i + 1 + j].x;
    (*vect)[j].coord.y = prob.pontos[i + 1 + j].y;
    (*vect)[j].key = 1; // 1 = caminho nao calculado
  }
}