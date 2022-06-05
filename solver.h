/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef SOLVER_H
#define SOLVER_H

#include "graph.h"
//A list to write down arguments that were changed
typedef struct ChangeList {
  //the length of the list
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

//Keep track of characteristics during the search
typedef struct SearchCharacteristics {
  int numberIntermediatePropagatied;
  int numberBlankPropagated;
  int numberErrors;
  int numberPropagated;

}SearchCharacteristics;

#include "heuristic.h"
#include "propagateLabels.h"

ChangeList createChangeList();
void freeChangeList(ChangeList changes);
void findComplete(Graph graph, bool printTree, char heuristic);
void findCompleteRec(Graph g,ChangeList *changes, int level, bool printTree, char heuristic, SearchCharacteristics *SC);
void updateSC(ChangeList changes, int currentNumberLabeled, SearchCharacteristics *SC);
void reverseChanges(ChangeList *changes, int till);

void addChange(ChangeList *li, Argument a, Label newLabel, int level, bool printTree, Cause cause);

void printInLevel(char * s, int level);
void printContradiction(Argument a, Label new_label,Cause cause, int level);

#endif