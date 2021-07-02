/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef SOLVER_H
#define SOLVER_H


#include "argumentList.h" //also includes "graph.h"
#include "heuristic.h"
#include "heap.h"

//A list to write down arguments that were changed
typedef struct ChangeList {
  //the number of arguments in the list
  int size;
  //the arguments
  Argument *arguments;
  //the label the argument had before
  Label *previousLabels;
  //the total number of argument in the list
  int totalLabeled;
   //the number of arguments having the label IN OUT or UNDEC
  int nFinalLabeled;
}ChangeList;

#include "propagateLabels.h"

ChangeList createChangeList();
void freeChangeList(ChangeList changes);

void findComplete(Graph graph, bool printTree, bool splitUnjustOuts);
void findCompleteRec(Graph g,ChangeList *changes, int level, bool printTree, Heap *heap, ArgumentList *unjustOuts, bool splitUnjustOuts);
void splitOverBlank(Graph g, Argument a,ChangeList *changes,int level,bool printTree, Heap *heap,ArgumentList *unjustOuts, bool splitUnjustOuts);
void splitOverUnjustifiedOut(Graph g, Argument a,ChangeList *changes,int level,bool printTree, Heap *heap,ArgumentList *unjustOuts);

Argument getNext(Heap *heap, Argument **finalArguments, int *finalIndex);

void updateHeuristic(ChangeList *li, int start, Heap *hp);
void reverseHeuristic(ChangeList *li, int start, Heap *hp);
void updateUnjustOutList(ChangeList *li, int start, ArgumentList *unjustOuts, bool splitUnjustOuts);
void reverseUnjustOutList(ChangeList *li, int end, ArgumentList *unjustOuts, bool splitUnjustOuts);

void addChange(ChangeList *li, Argument a, Label newLabel);
void reverseChanges(ChangeList *changes, int till);

void printInLevel(char * s, int level, bool printTree);
void printInLevel3(char * s, char * s2, char * s3, int level, bool printTree);

#endif