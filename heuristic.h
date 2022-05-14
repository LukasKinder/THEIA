/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef heuristic_H
#define heuristic_H

#include "graph.h"
#include "solver.h"

void printStatistics();
int getArgument(Graph g,ChangeList *changes, Label *labelSplit, Argument *bestArgument, int level, int printTree, char heuristic);
int lookAheadOption(Graph g,ChangeList *changes, Argument a, Label l, double *bestEstimatedTime, Argument *bestArgument,
         Label *labelSplit, int level, int printTree, char heuristic);
double estimateSolvingtime(ChangeList changes, char heuristic);

#endif