/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "argumentList.h"



//add an argument to the beginning of the list
ArgumentList addBeginningArgument(ArgumentList li, Argument a){
  if (a->nodeOfArgument != NULL){
    printf("error, argument already in List\n");
    exit(1);
  }

  ArgumentList newList= malloc(sizeof(struct ArgumentListNode));
  newList->argument = a;
  newList->previous = NULL;
  newList->next = li;
  if (li != NULL){
    li->previous = newList;
  }
  a->nodeOfArgument = newList;

  return newList;
}

//get and remove the first node of the list
ArgumentList getFirst(ArgumentList li, Argument *a){
  if (li ==NULL){
    printf("Error, list is empty\n");
    exit(1);
  }
  *a = li->argument;
  ArgumentList returnLi = li->next;
  free(li);
  if (returnLi != NULL){
    returnLi->previous = NULL;
  }
  (*a)->nodeOfArgument = NULL;
  return returnLi;
}

//remove a specific argument from the list
//works in O(1)
ArgumentList removeArgumentInList(ArgumentList li, Argument a){
  if (li ==NULL){
    if (a->nodeOfArgument != NULL){
      printf("but this\n");
    }
    printf("Error, list is empty 2\n");
    exit(1);
  }

  if (a->nodeOfArgument == NULL){
    printf("Error, Argument not in List\n");
    exit(1);
  }

  if (a->nodeOfArgument->previous == NULL && a->nodeOfArgument->next == NULL ){
    li = NULL;
  }else if(a->nodeOfArgument->previous == NULL){
    li = a->nodeOfArgument->next;
    a->nodeOfArgument->next->previous = NULL;
  }else if(a->nodeOfArgument->next == NULL){
    a->nodeOfArgument->previous->next = NULL;
  }else{
    a->nodeOfArgument->previous->next = a->nodeOfArgument->next;
    a->nodeOfArgument->next->previous = a->nodeOfArgument->previous;
  }
  free(a->nodeOfArgument);
  a->nodeOfArgument = NULL;

  return li;
}


void freeArgumentList(ArgumentList li){
  if (li ==NULL){
    return;
  }
  freeArgumentList(li->next);
  free(li);
}
