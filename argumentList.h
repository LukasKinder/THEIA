/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef ARGUMENTLIST_H
#define ARGUMENTLIST_H

/*A linked list of argument. Pointers to previous and next node are stored
in every node to enable removing any node from the list in O(1)*/
typedef struct ArgumentListNode* ArgumentList;

#include "graph.h"

struct ArgumentListNode {
  Argument argument;
  ArgumentList next;
  ArgumentList previous;
};

ArgumentList addBeginningArgument(ArgumentList li, Argument a);
ArgumentList getFirst(ArgumentList li, Argument *a);
ArgumentList removeArgumentInList(ArgumentList li, Argument a);
void freeArgumentList(ArgumentList li);

#endif