#include "utils.h"

//malloc com a verificação do erro
void* checked_malloc (size_t sz){
  void* mem = malloc(sz);
  if (mem == NULL){
    printf ("Out of memory!\n");
    exit(0);
  }
  return mem;
}
