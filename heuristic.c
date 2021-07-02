/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "heuristic.h"

#define ATTACK_BONUS 1 //Heuristic increse for each attack of an argument
#define ATTACKED_BONUS 0 //Heuristic increase for each argument attacking the argument

#define LABELED_NOTIN 10 //Heuristic increase for an argument labeled NOTIN
#define LABELED_NOTOUT 8  //Heuristic increase for an argument labeled NOTOUT

#define ATTACK_Attacking_Bonus 0.2  //Heuristic increase for each argument that is attacked by an argument attacked by the current argument

//gives each of the given arguments a heuristic value
void giveheuristics(Argument *arguments, int size){
  int i, j;
  Argument a;
  for (i = 0; i < size; i++){
    a = arguments[i];
    a->heuristic = a->numberIsAttacking * ATTACK_BONUS;
    a->heuristic = a->heuristic + a->numberAttackedBy * ATTACKED_BONUS;
    for (j = 0; j < a->numberIsAttacking; j++){
      a->heuristic =  a->heuristic + (a->isAttacking[j]->numberIsAttacking * ATTACK_Attacking_Bonus ) / a->numberIsAttacking;
    }
  } 
}

void labelFromBlankToNotIn(Argument a, Heap *hp){
  increaseHeuristic(a,LABELED_NOTIN,hp);
}

void labelFromBlankToNotOut(Argument a, Heap *hp){
  increaseHeuristic(a,LABELED_NOTOUT,hp);
}

void labelFromNotInToBlank(Argument a, Heap *hp){
  decreaseHeuristic(a,LABELED_NOTIN,hp);
}

void labelFromNotOutToBlank(Argument a, Heap *hp){
  decreaseHeuristic(a,LABELED_NOTOUT,hp);
}

void increaseHeuristic(Argument a, float change, Heap *hp){
  a->heuristic += change;
  upHeap(hp,a);
}

void decreaseHeuristic(Argument a, float change, Heap *hp){
  a->heuristic -= change;
  downHeap(hp,a);
}



