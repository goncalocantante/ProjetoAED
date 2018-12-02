#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct _heap
{
	int size;
	int free;
	int (*less)(Item, Item); //para comparar items
	Item *heapdata;
};


Heap *HeapInit(int (*less)(Item, Item), int size)
{
	Heap *_heap = (Heap *)checked_malloc(sizeof(Heap));
	_heap->size = size;
	_heap->free = 0;
	_heap->less = less;
	_heap->heapdata = (Item *)checked_malloc(sizeof(Item)*size);
	return _heap;
}

 
void HeapInsert(Heap *h, Item item)
{
	if ((h->free + 1) < h->size)
	{
		(h->heapdata)[h->free] = item;
	}
	FixUp(h, h->free);   
	(h->free)++;
}

void FixUp(Heap *h, int Idx)
{
	Item t;
	//enquanto o pai for menos prioritario que o filho
	while (Idx > 0 && (h->less)((h->heapdata)[(Idx - 1) / 2], (h->heapdata)[Idx]))
	{
		//Troca o filho com o pai
		t = (h->heapdata)[Idx]; //filho
		(h->heapdata)[Idx] = (h->heapdata)[(Idx - 1) / 2]; //filho = pai
		(h->heapdata)[(Idx - 1) / 2] = t; //pai = filho
		//Continua a subir do acervo
		Idx = (Idx - 1) / 2;
	}
	return;
}


void FixDown(Heap *h, int Idx)
{
	int Child; //indície do nó descendente
	int leaf = ((h->free) - 1);
	Item t;
		
	//enquanto não chega às folhas
	while (2 * Idx < leaf)
	{ 
		Child = 2 * Idx + 1;
		//seleciona o filho mais prioritário
		//se Child = leaf, significa é filho único
		if (Child < leaf && (h->less)((h->heapdata)[Child], (h->heapdata)[Child+1]))
			Child++;
		if ((h->less)((h->heapdata)[Child], (h->heapdata)[Idx])) // se o pai é mais prioritário que o filho
			break;						                         //condição de heap satisfeita

		//troca o pai com o filho mais prioritário
   		t = (h->heapdata)[Child];				   //filho
		(h->heapdata)[Child] = (h->heapdata)[Idx]; //filho = pai
		(h->heapdata)[Idx] = t;				       //pai = filho

		Idx = Child; //continua a descer na árvore
	}
}

void ChangePri(Heap *h, int index, Item item)
{
	//se o index não estiver dentro dos limites
	if (index > h->free - 1)
	{
		exit(0);
	}

  	if (index == -1) { printf("Indicie nao encontrado"); exit(0); }  

	//se aumentar a prioridade faz FixUp
	if ((h->less)((h->heapdata)[index], item))
	{	
		free((h->heapdata)[index]);
		(h->heapdata)[index] = item;
		FixUp(h, index);
	}
	else
	{ //se diminuir a prioridade faz FixDown
		free((h->heapdata)[index]);
		(h->heapdata)[index] = item;
		FixDown(h, index);
	}
} 

int EmptyHeap(Heap *h)
{
	return ((h->heapdata)[0] == NULL ? 1 : 0);
}

void HeapDeleteMostPri(Heap *h)
{
	Item t;
	//troca primeiro com o último
	t = (h->heapdata)[0];			   	   
	(h->heapdata)[0] = (h->heapdata)[(h->free)-1]; 
	(h->heapdata)[(h->free) - 1] = t;
	//elimina o ultimo
	free((h->heapdata)[(h->free) - 1]);
	(h->heapdata)[(h->free) - 1] = NULL;
	(h->free)--;
	//repoe a condiçao de heap
	FixDown(h, 0);
}


Item getMostPri(Heap *heap){
	return (heap->heapdata[0]);
}

/*void printQueue(Heap *h)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d\n", queue[i]->key);
	}
} */

Item getItem (Heap* h, int idx){
	if (idx > (h->free)-1) exit(0);	
	return h->heapdata[idx];
}

int getFree(Heap * h){
	return h->free;
}

int getSize(Heap *h)
{
	return h->size;
}