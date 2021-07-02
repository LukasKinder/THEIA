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
  UNJUSTIFIED_OUT,
  UNDEC,
  NOTOUT,
  NOTIN
} Label;


typedef struct Node *Argument;

//The graph contains the number of arguments and the arguments
typedef struct Graph {
  int numberArguments;
  Argument *arguments;
}Graph;

#include "argumentList.h"

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

  //heuristic value of argument. An argument with the highest heuristic value is choosen to split the search.
  float heuristic;

  //Index in heap of argument 
  int indexInHeap;

  /*Pointer of node in argument list that contains this argument. NULL if argument is not 
  contained in Argument list */
  ArgumentList nodeOfArgument;

} Node;

Graph createGraph(char * filename, float *averageAttacks);
void addSelfAttack(Argument a);
void addAttackedBy(Argument a, Argument b);
void addIsAttacking(Argument a, Argument b);
Argument newArgument(char *name);
void freeGraph(Graph g);
void freeArgument(Argument a);

void printLabel(Label l);

void printInArguments(Graph graph);
void removeLinebreak(char *str, int end);

#include "dictionary.h"

#endif