/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef heuristic_H
#define heuristic_H


#include "graph.h"
#include "heap.h"


void giveheuristics(Argument *arguments, int size);

void labelFromBlankToNotIn(Argument a, Heap *hp);
void labelFromBlankToNotOut(Argument a, Heap *hp);
void labelFromNotInToBlank(Argument a, Heap *hp);
void labelFromNotOutToBlank(Argument a, Heap *hp);

void increaseHeuristic(Argument a, float change, Heap *hp);
void decreaseHeuristic(Argument a, float change, Heap *hp);

#endif