#ifndef QUEUE_H
#define QUEUE_H

#include "utils.h"
#include <stdbool.h>

typedef struct {
	int key;
	int x,y;
} Heap;


Heap** HeapInit (int);
void HeapInsert (Heap** , Heap *, int *, int );
void FixUp (Heap **, int );
void exch(Heap *, Heap *);
int EmptyHeap (Heap **);
void PriorityDec(int , int , Heap **, int , int );

#endif