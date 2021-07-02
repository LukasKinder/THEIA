/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

/*Code is largely copied from the rug course "Algorithm and data structures"*/

#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include "graph.h"


typedef struct Heap{
  Argument *array;
  int front;
  int size;
}Heap;

Heap makeHeap(int size, Argument *arguments);
void doubleHeapSize(Heap *hp);
void enqueue (Argument a, Heap *hp);
Argument getMax(Heap *Hp);
void upHeap(Heap *hp, Argument a);
void downHeap(Heap *hp, Argument a);
void freeHeap(Heap *hp);
void removeFromHeap(Argument a, Heap *hp);

#endif