#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourist.h"
#include "modos.h"

//liberta memória alocada na estrutura
void free_struct(dados *estrutura){
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

//verifica extensão (.cities) do programa
//verifica o número de argumentos da chamada do programa
void test_file (char nomefich[], int argc){
  char extensao[MAXSTR] = ".cities";
  char ext_fich[MAXSTR];
  int i, j = 0;

  for (i = strlen(nomefich) - 7; i <= strlen(nomefich); i++) { // 7 = strlen de .cities
    ext_fich[j] = nomefich[i];
    j++;
  }

  if (argc != 2) {
    exit(0);
  }
  if (strcmp(ext_fich, extensao) != 0) {
    exit(0);
  }
}

//malloc com a verificação do erro
void* checked_malloc (size_t sz){
  void* mem = malloc(sz);
  if (mem == NULL){
    printf ("Out of memory!\n");
    exit(0);
  }
  return mem;
}

dados *ler_problema(char nomefich[] ,FILE *fp){
  int i = 0, j = 0;
  dados *prob = NULL;

  prob = (dados *)checked_malloc(sizeof(dados));
  prob->modo = '0';

  //retira os dados da primeira linha do ficheiro
  fscanf (fp, "%d %d %c %d", &prob->nlinhas ,&prob->ncolunas, &prob->modo, &prob->npontos);
  if (prob->modo == '0') {     //indica se há outro problema ou não
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

int validate_exercise(dados prob){
  int ex_valido = 1;

  if ((prob.modo != 'A' && prob.modo != 'B') || prob.nlinhas <= 0 || prob.ncolunas <= 0
      || prob.npontos <= 0 || (prob.modo == 'A' && prob.npontos != 1)) {
        ex_valido = 0;
  }
  return ex_valido;
}
//sdfd
int validate_points(dados prob){
  int i = 0, ponto_valido = 1;

  for (i = 0; i < prob.npontos; i++) {
    if (((prob.pontos[i][0] >= prob.nlinhas) || (prob.pontos[i][1] >= prob.ncolunas))) {
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

solucao *solve_problem(dados prob) {
  solucao *sol = NULL;

  if (validate_exercise(prob) == 0 || validate_points(prob) == 0) {
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
