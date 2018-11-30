#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define lessPri(A, B) (A->key > B->key)


Heap** HeapInit (int size){
	int i;
	Heap **queue = checked_malloc(sizeof(Heap*)*size);
	for (i = 0; i < size; i++)
		queue[i] = NULL;
	return queue;
}

void HeapInsert (Heap** queue, Heap *I, int free, int heap_size){
	if ( (free+1) < heap_size ){
		queue[free] = I;
	}
	FixUp(queue, free);
	free++;
}

void FixUp (Heap **queue, int Idx){
	//enquanto o pai for menos prioritario que o filho
	while(Idx > 0 && lessPri(queue[(Idx-1)/2], queue[Idx])){
		exch( queue[Idx], queue[(Idx-1)/2]);
		//sobe no acervo
		Idx = (Idx-1)/2;
	}
}

void exch(Heap *A, Heap *B){
	int key = A->key, x = A->x, y = A->y;

	A->key = B->key;
	A->x = B->x;
	A->y = B->y;
		
	B->key = key;
	B->x = x;
	B->y = y;
}

void PriorityDec(int x, int y, Heap **heap, int key, int size){
	int i;
	//procura o ponto no heap
	for (i = 0; i < size; i++){
		if (heap[i]->x == x && heap[i]->y == y){
			//altera a key e repoe a ordem do heap
			heap[i]->key = key;
			Fixup(heap, i);
			break;
		}
	}
}

BOOL EmptyHeap (Heap **queue){
	return (queue[0] == NULL ? TRUE : FALSE);
}

void HeapDeleteMin(Heap **heap, int *free){
		//troca primeiro com o último
		exch(heap[0], heap[*free-1]);
		//elimina o ultimo
		free(heap[*free-1]);
		heap[*free-1] = NULL;
		*free = *free - 1;
		//FixDown();		
}

void FixDown (Heap **queue, int Idx){
	//enquanto o pai for menos prioritario que o filho
	while(Idx > 0 && lessPri(queue[(Idx-1)/2], queue[Idx])){
		exch( queue[Idx], queue[(Idx-1)/2]);
		//sobe no acervo
		Idx = (Idx-1)/2;
	}
}