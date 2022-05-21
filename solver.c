/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

//create empty change list
ChangeList createChangeList(int size){
  ChangeList l;
  l.size = size;
  l.arguments = malloc(sizeof(Argument)*size);
  l.previousLabels = malloc(sizeof(Label)*size);
  l.totalLabeled = 0;
  l.nFinalLabeled = 0;
  return l;
}

void freeChangeList(ChangeList changes){
  free(changes.arguments);
  free(changes.previousLabels);
}

//finds all complete extensions and prints them
void findComplete(Graph g, bool printTree, char heuristic){
  ChangeList changes = createChangeList(g.numberArguments);
  int i;
  Argument a;
  printf("[\n");
  for (i =0; i < g.numberArguments; i++){
    a = g.arguments[i];
    if (a->numberAttackedBy == 0 && !a->selfAttack){
      //a does not have an attacker so it should be labeled IN
      if (!setIn(a, &changes,0,printTree, basecase1)){
        printf("Error: applying base cases should not lead to contradictions!");
        exit(1); //creating the grounded extension should not create an error
      }  
    }
    if (a->selfAttack){
      //a does not have an attacker so it should be labeled IN
      if (!setNotIn(a, &changes,0,printTree, basecase2)){
        printf("Error: applying base cases should not lead to contradictions!");
        exit(1); //creating the grounded extension should not create an error
      }  
    }
    if (a->numberAttackedBy == 0 && a->selfAttack){
      //a does not have an attacker so it should be labeled IN
      if (!setUndec(a, &changes,0,printTree, basecase3)){
        printf("Error: applying base cases should not lead to contradictions!");
        exit(1); //creating the grounded extension should not create an error
      }  
    }
  }

  //printf("Grounded extension: ");
  //printInArguments(g);
  
  findCompleteRec(g, &changes, 0, printTree, heuristic);
  printf("]\n");
  if (printTree) printStatistics();
  freeChangeList(changes);
}

/*Function that checks if a solution is found, otherwise an argument is chosen to split the search*/
void findCompleteRec(Graph g,ChangeList *changes, int level, bool printTree, char heuristic){

  Argument a = NULL;
  Label labelToSplit;
  int c = 0;
  while (a == NULL){
    c++;
    if (!getArgument(g,changes,&labelToSplit, &a, level, printTree, heuristic)){
      //contradiction found through look-ahead
      return;
    }
    if (changes->nFinalLabeled == g.numberArguments){
      //all arguments are labeled - a complete extension is found
      if (printTree) printInLevel("Solution found:",level);
      printInArguments(g);
      return;
    }
    if (c> 100){
      printf("ERROR: forward checking could not find argument!\n");
      exit(1);
    }
  }

  if (printTree) {printInLevel("Split search over argument ",level); printf("%s\n", a->name);}
  

  int currentNumberLabeled = changes->totalLabeled;
  if (setArgument(a, labelToSplit,changes,level + 1,printTree, splitting_search)){
    findCompleteRec(g,changes,level +1, printTree, heuristic);
  }
  reverseChanges(changes, currentNumberLabeled); 

  if (setArgument(a, oppositLabel(labelToSplit),changes,level + 1,printTree, splitting_search)){
    findCompleteRec(g,changes,level +1, printTree, heuristic);
  }
  reverseChanges(changes, currentNumberLabeled);
}


//labeles the arguments to BLANK such that only the first "till" arguments are still labeled
void reverseChanges(ChangeList *changes, int till){
  Argument a;
  int i;
  while(till < changes->totalLabeled){
    changes->totalLabeled--;
    a = changes->arguments[changes->totalLabeled];
    if (a->label == IN || a->label == OUT || a->label == UNDEC){
      changes->nFinalLabeled--;
    }

    switch (a->label){
      case OUT:
        for (i = 0; i < a->numberIsAttacking; i++){
          a->isAttacking[i]->attackedByOut --;
        }
        break;
      case UNDEC:
        for (i = 0; i < a->numberIsAttacking; i++){
          a->isAttacking[i]->attackedByUndec --;
        }
        break;
      case NOTIN:
        for (i = 0; i < a->numberIsAttacking; i++){
          a->isAttacking[i]->attackedByNotIn--;
        }
        break;
      case NOTUNDEC:
        for (i = 0; i < a->numberIsAttacking; i++){
          a->isAttacking[i]->attackedByNotUndec--;
        }
    }

    //label the argument to the old label again
    a->label = changes->previousLabels[changes->totalLabeled];

    switch (a->label){
      case NOTIN:
        for (i = 0; i < a->numberIsAttacking; i++){
          a->isAttacking[i]->attackedByNotIn++;
        }
        break;
      case NOTUNDEC:
        for (i = 0; i < a->numberIsAttacking; i++){
          a->isAttacking[i]->attackedByNotUndec++;
        }
    }
  }
}

//prints a line with the correct number of leading spaces.
void printInLevel(char * s, int level){
  for (int i = 0; i < level; i++){
    printf("   ");
  }
  printf("%s",s);
}

void printContradiction(Argument a, Label new_label,Cause cause, int level){
  printInLevel("Argument ",level);
  printf("%s gets labeled ", a->name);
  printLabel(new_label);
  printf(" %s, which is a contradiction, because it is already labeled ", stringOfCause(cause));
  printLabel(a->label);
}




//add an argument in the list that was changed and relabeles the argument
void addChange(ChangeList *li, Argument a, Label newLabel, int level, bool printTree, Cause cause){

  if (printTree){
    printInLevel(a->name,level);
    printf(" = ");
    printLabel(newLabel);
    printf(" %s\n",stringOfCause(cause));
  }

  if (li->size == li->totalLabeled){
    li->size *=2;
    li->arguments = realloc(li->arguments, li->size * sizeof(Argument));
    li->previousLabels = realloc(li->previousLabels, li->size * sizeof(Label));
  }

  switch (a->label){
  case NOTIN:
    for (int i = 0; i < a->numberIsAttacking; i++){
      a->isAttacking[i]->attackedByNotIn--;
    }
    break;
  case NOTUNDEC:
    for (int i = 0; i < a->numberIsAttacking; i++){
        a->isAttacking[i]->attackedByNotUndec--;
      }
  }

  switch (newLabel){
  case NOTIN:
    for (int i = 0; i < a->numberIsAttacking; i++){
      a->isAttacking[i]->attackedByNotIn++;
    }
    break;
  case NOTUNDEC:
    for (int i = 0; i < a->numberIsAttacking; i++){
      a->isAttacking[i]->attackedByNotUndec++;
    }
    break;
  case OUT:
    for (int i = 0; i < a->numberIsAttacking; i++){
      a->isAttacking[i]->attackedByOut++;
    }
    break;
  case UNDEC:
    for (int i = 0; i < a->numberIsAttacking; i++){
      a->isAttacking[i]->attackedByUndec++;
    }
    break;
  default:
    break;
  }

  li->arguments[li->totalLabeled] = a;
  li->previousLabels[li->totalLabeled] = a->label;
  li->totalLabeled++;
  a->label = newLabel;
  if (newLabel == IN || newLabel == OUT || newLabel == UNDEC){
    li->nFinalLabeled++;
  }
}








