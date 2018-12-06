#ifndef QUEUE_H
#define QUEUE_H

#include "utils.h"

typedef struct _heap Heap;

Heap *HeapInit(int (*less)(Item, Item), int (*GetId)(Item, int), int , int);
void HeapInsert(Heap *, Item);
void FixUp(Heap *, int);
void FixDown(Heap *, int);
void ChangePri(Heap *h, Item);
int EmptyHeap(Heap *);
Item getItem(Heap *, int);
int getFree(Heap *);
Item getMostPri(Heap *);
void HeapDeleteMostPri(Heap *);
Item *getHeapData(Heap *);
void freeHeap(Heap *);

#endif