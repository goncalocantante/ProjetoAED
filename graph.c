#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

graph* CreateGraph (problema prob){
	graph *grafo = (graph*)checkedmalloc(sizeof(graph)); 
	Vertice *vertice = NULL;
	int *moves;
	int i, j, k = 0;

	for(i = 0; i < prob.nlinhas; i++){
		for(j = 0; j < prob.ncolunas; j++){


			//se ponto válido
			if(prob.mapa[i][j] != 0){
				
				estrututra[k]->k = k;
				estrutura->x 
				//vamos por no grafo e guardar os seus adjacentes
				vertice = (Vertice*)checkedmalloc(sizeof(Vertice));
				vertice->V[0] = i;
				vertice->V[1] = j; 

				graph->ladjs[k] = ; 




				//chamar a funçao para dar as oordenadas dos adjacentes
				//pomos os adjantes na lista de indice [i][j];
			k++;
			}	
		}
	}
	

}

void  GetAdjVerts (problema prob, int x, int y){
	//movimentações possiveis
	int moves[8][2]= {{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
	int i, j:

	for (i = 0; i < 8; i++){
		for (j = 0; j < 2; j++){
			if (prob.mapa[x][y] != 0){

			}
		}

	}

}