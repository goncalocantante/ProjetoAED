//modoB.c

#include <stdio.h>
#include <stdlib.h>

#include "modos.h"
#include "tourist.h"


solucao *modoB (dados prob) {
  int i = 0, x, y, nx, ny;
  solucao *B;

  B = (solucao *)checked_malloc(sizeof(solucao));
  B->custo = 0;
  B->valido = -1;

  for (i = 0; i < prob.npontos - 1; i++) {
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

    if (B->valido == -1 || prob.mapa[x][y] == 0 || prob.mapa[nx][ny] == 0) {
      B->custo = 0;
      B->valido = -1;
      return B;
    }
    B->custo += prob.mapa[nx][ny];
  }
  return B;
}
