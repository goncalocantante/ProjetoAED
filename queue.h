#ifndef QUEUE_H
#define QUEUE_H

#include "utils.h"

typedef struct _heap Heap;

Heap *HeapInit(int (*less)(Item, Item), int );
void HeapInsert(Heap *, Item);
void FixUp(Heap *, int);
void FixDown(Heap *, int);
void ChangePri(Heap *h, int, Item);
int EmptyHeap(Heap *);
Item getItem(Heap *, int);
int getFree(Heap *h);
Item getMostPri(Heap *);
void HeapDeleteMostPri(Heap *);
/* void FixUp(Heap **, int);
void exch(Heap *, Heap *);
int EmptyHeap(Heap **);
void PriorityDec(int, int, Heap **, int, int);
void printQueue(Heap **, int);
void FixDown(Heap **, int, int); */

#endif