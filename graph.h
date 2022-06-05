/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <string.h>

//Labels for algorithm, BLANK stands for "no label"
typedef enum Label {
  BLANK,
  IN,
  OUT,
  UNDEC,
  NOTOUT,
  NOTIN,
  NOTUNDEC
} Label;

//Call causes to label an argument
typedef enum Cause {
  basecase1,basecase2,basecase3,
  rule1,rule2,rule3,rule4,rule5,rule6,rule7,rule8,rule9,rule10,rule11,rule12,rule13,rule14,rule15,rule16,
  look_ahead, 
  splitting_search
} Cause;

static inline char *stringOfCause(enum Cause cause){
    static char *strings[] = { 
      "by basecase 1", "by basecase 2", "by basecase 3", 
      "by rule 1","by rule 2","by rule 3","by rule 4","by rule 5","by rule 6","by rule 7","by rule 8","by rule 9","by rule10","by rule11","by rule12","by rule13","by rule14","by rule15","by rule16",
      "label enforced by look-ahead", 
      "label caused by splitting the search"
    };
    return strings[cause];
}


typedef struct Node *Argument;

//The graph contains the number of arguments and the arguments
typedef struct Graph {
  int numberArguments;
  Argument *arguments;
}Graph;

//struct for an argument
typedef struct Node {
  //the name of the argument
  char *name;
  //the label of the argument
  Label label;

  //if the argument is self attacking
  bool selfAttack;
  //number of arguments attacking it that are labeled OUT
  int attackedByOut; 
  //number of arguments attacking it that are labeled UNDEC
  int attackedByUndec; 
  //number of arguments attacking it that are labeled NOTIN
  int attackedByNotIn; 
  //number of arguments attacking it that are labeled NOTUNDEC
  int attackedByNotUndec; 
  //number of argument that attack this argument
  int numberAttackedBy;
  //number of argument that are attacked by this argument
  int numberIsAttacking;
  //the arguments that it is attacking
  Argument *isAttacking;
  //the argument which attack this argument
  Argument *attackedBy;
  //size of the array "isAttacking"
  int sizeIsAttackingArray;
  //size of the array "attackedBy"
  int sizeAttackedByArray;

} Node;

int cmpfunc(const void * a, const void * b);

Graph createGraph(char * filename);
void addSelfAttack(Argument a);
void addAttackedBy(Argument a, Argument b);
void addIsAttacking(Argument a, Argument b);
Argument newArgument(char *name);
void freeGraph(Graph g);
void freeArgument(Argument a);

void printLabel(Label l);

void printInArguments(Graph graph);
void removeLinebreak(char *str, int end);

Label oppositLabel(Label l);
void printState(Graph g);

#include "dictionary.h"

#endif