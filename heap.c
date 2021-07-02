/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

//creates heap conataining all arguments given.
Heap makeHeap(int size, Argument *arguments){
  Heap h;
  h.array = malloc((size +1)* sizeof (Argument));
  h.front = 1;
  h.size = size + 1;
  for (int i = 0; i < size ; i++){
    if (arguments[i]->label != IN && arguments[i]->label != OUT && arguments[i]->label != UNDEC){
      enqueue(arguments[i], &h);
    }
  }
  return h;
}

void doubleHeapSize(Heap *hp){
  hp->size *= 2;
  hp->array = realloc((*hp).array, sizeof(Argument) * (*hp).size); 
}

void enqueue (Argument a, Heap *hp){
  if(hp->front == hp->size){
    doubleHeapSize(hp);
  }
  a->indexInHeap = hp->front;
  hp->array[hp->front] = a;
  hp->front++;

  upHeap(hp, a);
}


Argument getMax(Heap *Hp){
  if (Hp->front == 1){
    printf("ERROR: Heap is empty!\n");
    exit(1);
  }
  Argument a = Hp->array[1];
  return a;
}

void upHeap(Heap *hp, Argument a){
  int n = a->indexInHeap;
  if (n > 1 && (hp->array[n]->heuristic) > (hp->array[n/2])->heuristic){
    //we have to swap
    Argument b = hp->array[n/2];
    b->indexInHeap  = n;
    a->indexInHeap /=2;
    hp->array[n] = b;
    hp->array[n/2] = a;
    upHeap(hp, a);
  }
}

void removeFromHeap(Argument a, Heap *hp){
  if (a->name[0] != hp->array[a->indexInHeap]->name[0]){
    printf("ERROR: argument '%s' does not exist in position %d\n",a->name, a->indexInHeap);
    exit(1);
  }

  if (a->indexInHeap < 1 || a->indexInHeap >=hp->front){
    printf("ERROR: argument '%s' does not exist in heap (index = %d, front = %d)\n",a->name, a->indexInHeap,hp->front);
    exit(1);
  }

  hp->front--;

  if (hp->front != a->indexInHeap){
    Argument b = (hp->array)[hp->front];
    b->indexInHeap = a->indexInHeap;
    hp->array[a->indexInHeap] = b;
    //make sure that the parent is always bigger than both children
    downHeap(hp, b);
    upHeap(hp,b); 
  }
  a->indexInHeap = -1;
}

void downHeap(Heap *hp, Argument a){
  int n = a->indexInHeap;
  float heuristic = a->heuristic;
  if (n*2 >= (hp->front)){
    return;
  }

  float lc = hp->array[n*2]->heuristic;
  float rc = n*2 +1 == hp->front ?  -10000000 : hp->array[(n*2)+1]->heuristic;
  if ((lc > heuristic) && (lc > rc)){
    Argument b = hp->array[n*2];
    b->indexInHeap =n;
    a->indexInHeap *=2;
    hp->array[n] = b;
    hp->array[n*2] = a;
    downHeap(hp, a);
  }else if(rc > heuristic){
    Argument b = hp->array[n*2 +1];
    b->indexInHeap = n;
    a->indexInHeap = a->indexInHeap * 2 +1;
    hp->array[n] = b;
    hp->array[n*2+1] = a;
    downHeap(hp, a);
  }
}

void freeHeap(Heap *hp){
  free(hp->array);
}



