/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

int usedUNJOUT = 0;
int usedBlank = 0;

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

//finds all stable extensions and prints them
void findComplete(Graph g, bool printTree, bool splitUnjustOuts){
  ChangeList changes = createChangeList(g.numberArguments);
  ArgumentList unjustOuts = NULL;

  int i;
  Argument a;
  for (i =0; i < g.numberArguments; i++){
    a = g.arguments[i];
    if (a->numberAttackedBy == 0 && !a->selfAttack){
      //a does not have an attacker so it should be labeled IN
      if (!setIn(a, &changes,0,printTree,-1)){
        printf("GROUNDED SHOULD NOT LEAD TO ERROR\n");
        exit(1);
      }  
    }
  }

  //By now we have the grounded extension
  //printf("Grounded extension: ");
  //printInArguments(g);

  giveheuristics(g.arguments,g.numberArguments);
  Heap heap = makeHeap(g.numberArguments, g.arguments);

  int start = changes.totalLabeled;

  for (i =0; i < g.numberArguments; i++){
    a = g.arguments[i];
    if (a->numberAttackedBy == a->attackedByOut && a->selfAttack){
      //a does not have an attacker but attacks itself, it can only Have the UNDEC label
      if (!setUndec(a, &changes,0,printTree,-2)){
        printf("GROUNDED 2 SHOULD NOT LEAD TO ERROR\n");
        exit(1);
      }  
    }
    if (a->selfAttack){
      //self attacking arguments can not be IN
      if (!setNotIn(a, &changes,0,printTree,-3)){
        printf("This should not give an error\n");
        exit(1);
      }  
    }
  }
  updateHeuristic(&changes,start, &heap);
  
  //printf("Complete extensions: \n");


  //recursively label a random argument IN and OUT
  findCompleteRec(g, &changes, 0, printTree, &heap, &unjustOuts, splitUnjustOuts);

  freeChangeList(changes);
  freeHeap(&heap);
  if (unjustOuts != NULL){
    printf("should not happen unjustOut\n");
    exit(0);
  }
  printf("split over UNJUST_OUT: %d times\n",usedUNJOUT);
  printf("split search total: %d times\n",usedBlank + usedUNJOUT);
}

/*Function that picks some argument and sets it to IN and OUT and checks if assignment can
be made legal*/
void findCompleteRec(Graph g,ChangeList *changes, int level, bool printTree, Heap *heap,ArgumentList *unjustOuts, bool splitUnjustOuts){
  if (changes->nFinalLabeled == g.numberArguments){
    //all arguments are labeled - a complete extension is found
    printInLevel("Solution found:",level,printTree);
    printInArguments(g);
    return;
  }
  Argument a;
  if (splitUnjustOuts && *unjustOuts != NULL){
    usedUNJOUT++;
    *unjustOuts = getFirst(*unjustOuts, &a);
    splitOverUnjustifiedOut(g,a,changes,level,printTree, heap, unjustOuts);
  }else{
    usedBlank++;
    int size;
    Argument *finalArguments = NULL;
    a = getNext(heap, &finalArguments, &size);
    splitOverBlank(g,a,changes,level,printTree, heap, unjustOuts, unjustOuts);
    enqueue(a,heap);
    if (finalArguments != NULL){
      for (int i = 0; i < size; i++){
        enqueue(finalArguments[i],heap);
      }
      free(finalArguments);
  }
  }
}

/*can be used if argument a is labeled UNJUSTIFIED_OUT and has only two attackers 'b' and 'c' that can be IN
labels 'b' and 'c' IN 
or 'b' = IN and 'c' = NOTIN 
or 'b' = NOTIN and 'c' = IN
Andpropagates the labeled through the graph respectively.  */
void splitOverUnjustifiedOut(Graph g, Argument a, ChangeList *changes,int level,bool printTree, Heap *heap,ArgumentList *unjustOuts){

  printInLevel3("Split search over UNJUSTIFIED_OUT argument ",a->name,"\n", level , printTree);
  
  int currentNumberLabeled = changes->totalLabeled;
  int maxLabeled;

  Argument b,c;
  bool foundB = false;
  for (int i = 0; i < a->numberAttackedBy; i++){
    if (!foundB){
      b= a->attackedBy[i];
      if (b->label == BLANK || b->label == NOTOUT){
        foundB = true;
      }
    }else{
      c = a->attackedBy[i];
      if (c->label == BLANK || c->label == NOTOUT){
        break;
      }
    } 
  }

  //'b' = IN and 'c' = IN
  printInLevel3("case ",b->name," = IN: ",level,printTree);
  printInLevel3("and ",c->name," = IN: \n",level,printTree);
  if (setIn(b,changes,level + 1,printTree, -5) && setIn(c,changes,level + 1,printTree, -5) ){
    updateHeuristic(changes,currentNumberLabeled,heap);
    updateUnjustOutList(changes,currentNumberLabeled, unjustOuts,true);
    findCompleteRec(g,changes,level +1, printTree,heap, unjustOuts,true);
    reverseHeuristic(changes,currentNumberLabeled,heap);
    maxLabeled = changes->totalLabeled;
    reverseChanges(changes, currentNumberLabeled);
    reverseUnjustOutList(changes,maxLabeled, unjustOuts,true);
  }else{
    reverseChanges(changes, currentNumberLabeled);
  }

  //'b' = IN and 'c' = NOTIN
  printInLevel3("case ",b->name," = IN: ",level,printTree);
  printInLevel3("and ",c->name," = NOTIN: \n",level,printTree);
  if (setIn(b,changes,level + 1,printTree, -5) && setNotIn(c,changes,level + 1,printTree, -5) ){
    updateHeuristic(changes,currentNumberLabeled,heap);
    updateUnjustOutList(changes,currentNumberLabeled, unjustOuts,true);
    findCompleteRec(g,changes,level +1, printTree,heap, unjustOuts,true);
    reverseHeuristic(changes,currentNumberLabeled,heap);
    maxLabeled = changes->totalLabeled;
    reverseChanges(changes, currentNumberLabeled);
    reverseUnjustOutList(changes,maxLabeled, unjustOuts,true);
  }else{
    reverseChanges(changes, currentNumberLabeled);
  }

  //'b' = NOTIN and 'c' = IN
  printInLevel3("case ",b->name," = NOTIN: ",level,printTree);
  printInLevel3("and ",c->name," = IN: \n",level,printTree);
  if (setNotIn(b,changes,level + 1,printTree, -5) && setIn(c,changes,level + 1,printTree, -5) ){
    updateHeuristic(changes,currentNumberLabeled,heap);
    updateUnjustOutList(changes,currentNumberLabeled, unjustOuts,true);
    findCompleteRec(g,changes,level +1, printTree,heap, unjustOuts,true);
    reverseHeuristic(changes,currentNumberLabeled,heap);
    maxLabeled = changes->totalLabeled;
    reverseChanges(changes, currentNumberLabeled);
    reverseUnjustOutList(changes,maxLabeled, unjustOuts,true);
  }else{
    reverseChanges(changes, currentNumberLabeled);
  }


}

//labeles argument a IN, OUT or UNDEC an propagates the labeled through the graph.
void splitOverBlank(Graph g, Argument a,ChangeList *changes,int level,bool printTree, Heap *heap,ArgumentList *unjustOuts, bool splitUnjustOuts){
  printInLevel3( "Split search over BLANK argument ",a->name,"\n", level , printTree);
  
  if (a->label != BLANK && a->label != NOTOUT && a->label != NOTIN){
    printf("ERROR: none BLANK label  !!!\n");
    printLabel(a->label);
    exit(1);
  }
  
  printInLevel3("case ",a->name," = IN:\n",level,printTree);
  //label the argument IN
  int currentNumberLabeled = changes->totalLabeled;
  int maxLabeled;
  if (setIn(a,changes,level + 1,printTree, -4)){
    updateHeuristic(changes,currentNumberLabeled,heap);
    updateUnjustOutList(changes,currentNumberLabeled, unjustOuts,splitUnjustOuts);
    findCompleteRec(g,changes,level +1, printTree,heap, unjustOuts,splitUnjustOuts);
    reverseHeuristic(changes,currentNumberLabeled,heap);
    maxLabeled = changes->totalLabeled;
    reverseChanges(changes, currentNumberLabeled);
    reverseUnjustOutList(changes,maxLabeled, unjustOuts,splitUnjustOuts);
  }else{
    reverseChanges(changes, currentNumberLabeled);
  }


  printInLevel3("case ",a->name," = OUT:\n",level,printTree);
  //label the argument OUT
  if (setOut(a,changes,level + 1,printTree, false, -4)){
    updateHeuristic(changes,currentNumberLabeled,heap);
    updateUnjustOutList(changes,currentNumberLabeled, unjustOuts,splitUnjustOuts);
    findCompleteRec(g,changes,level +1, printTree, heap, unjustOuts,splitUnjustOuts);
    reverseHeuristic(changes,currentNumberLabeled,heap);
    maxLabeled = changes->totalLabeled;
    reverseChanges(changes, currentNumberLabeled);
    reverseUnjustOutList(changes,maxLabeled, unjustOuts,splitUnjustOuts);
  }else{
    reverseChanges(changes, currentNumberLabeled);
  }

  printInLevel3("case ",a->name," = UNDEC:\n",level,printTree);
  //label the argument UNDEC
  if (setUndec(a,changes,level + 1,printTree, -4)){
    updateHeuristic(changes,currentNumberLabeled,heap);
    updateUnjustOutList(changes,currentNumberLabeled, unjustOuts,splitUnjustOuts);
    findCompleteRec(g,changes,level + 1, printTree, heap, unjustOuts,splitUnjustOuts);
    reverseHeuristic(changes,currentNumberLabeled,heap);
    maxLabeled = changes->totalLabeled;
    reverseChanges(changes, currentNumberLabeled);
    reverseUnjustOutList(changes,maxLabeled, unjustOuts,splitUnjustOuts);
  }else{
    reverseChanges(changes, currentNumberLabeled);
  }
}

/* returns the argument from the heap that has the highest heuristic
and not the label IN OUT or UNDEC*/
Argument getNext(Heap *heap, Argument **finalArguments, int *finalIndex){
  Argument a;
  int size = 0, index = 0;
  a = getMax(heap);
  removeFromHeap(a,heap);
  while ( a->label != BLANK && a->label != NOTIN && a->label != NOTOUT ){
    if (*finalArguments == NULL){
      *finalArguments = malloc(sizeof(Argument));
      size = 1;
    }
    if (size == index){
      size *=2;
      *finalArguments = realloc(*finalArguments,sizeof(Argument)*size);
    }
    (*finalArguments)[index] = a;
    a = getMax(heap);
    removeFromHeap(a,heap);
    index ++;
  }
  *finalIndex = index;
  return a;
}

//updates the heuristic of arguments that got labeled NOTIN or NOTOUT
void updateHeuristic(ChangeList *li, int start, Heap *hp){
  Argument a;
  for (int i = start; i < li->totalLabeled; i++){
    a = li->arguments[i];
    if (a->label == NOTIN){
      labelFromBlankToNotIn(a,hp);
    }else if (a->label == NOTOUT){
      labelFromBlankToNotOut(a,hp);
    }
  }
}

//updates the heuristic of arguments that are not labeled NOTIN or NOTOUT anymore
void reverseHeuristic(ChangeList *li, int start, Heap *hp){
  Argument a;
  for (int i = start; i < li->totalLabeled; i++){
    a = li->arguments[i];
    if (a->label == NOTIN){
      labelFromNotInToBlank(a,hp);
    }else if (a->label == NOTOUT){
      labelFromNotOutToBlank(a,hp);
    }
  }
}

void updateUnjustOutList(ChangeList *li, int start, ArgumentList *unjustOuts, bool splitUnjustOuts){
  if (!splitUnjustOuts){
    return;
  }
  Argument a,b;
  int j;
  for (int i = start; i < li->totalLabeled; i++){
    a = li->arguments[i];
    if (a->label == UNJUSTIFIED_OUT && a->nodeOfArgument ==  NULL
          && a->numberAttackedBy - a->attackedByOut - a->attackedByUndec - a->attackedByNotIn == 2){
      *unjustOuts = addBeginningArgument(*unjustOuts,a);
    }
    
    if(a->label != UNJUSTIFIED_OUT && a->nodeOfArgument != NULL){
      *unjustOuts = removeArgumentInList(*unjustOuts, a);
    }
    if (a->label == OUT || a->label == UNJUSTIFIED_OUT || a->label == NOTIN || a->label == UNDEC){
      for (j = 0; j < a->numberIsAttacking; j++){
        b = a->isAttacking[j];
        if (b->nodeOfArgument == NULL && b->label == UNJUSTIFIED_OUT
              &&  b->numberAttackedBy - b->attackedByOut - b->attackedByUndec - b->attackedByNotIn == 2){
          *unjustOuts = addBeginningArgument(*unjustOuts,b);      
        }
      }
    }
  }
}

void reverseUnjustOutList(ChangeList *li, int end, ArgumentList *unjustOuts,bool splitUnjustOuts){
  if (!splitUnjustOuts){
    return;
  }
  Argument a,b;
  int j;
  for (int i = li->totalLabeled; i < end; i++){
    a = li->arguments[i];
    if (a->label == UNJUSTIFIED_OUT && a->nodeOfArgument ==  NULL
          && a->numberAttackedBy - a->attackedByOut - a->attackedByUndec - a->attackedByNotIn == 2){
      *unjustOuts = addBeginningArgument(*unjustOuts,a);
    }
    
    if(a->label != UNJUSTIFIED_OUT && a->nodeOfArgument != NULL){
      *unjustOuts = removeArgumentInList(*unjustOuts, a);
    }

    if (a->label == BLANK || a->label==NOTOUT){
      for (j = 0; j < a->numberIsAttacking; j++){
        b = a->isAttacking[j];
        if (b->nodeOfArgument != NULL
              &&  b->numberAttackedBy - b->attackedByOut - b->attackedByUndec - b->attackedByNotIn != 2){
          *unjustOuts = removeArgumentInList(*unjustOuts, b);   
        }
      }
    }
  }
}


//add an argument in the list that was changed and relabeles the argument
void addChange(ChangeList *li, Argument a, Label newLabel){
  if (li->size == li->totalLabeled){
    li->size *=2;
    li->arguments = realloc(li->arguments, li->size * sizeof(Argument));
    li->previousLabels = realloc(li->previousLabels, li->size * sizeof(Label));
  }

  switch (newLabel){
  case NOTIN:
    for (int i = 0; i < a->numberIsAttacking; i++){
      a->isAttacking[i]->attackedByNotIn++;
    }
    break;
  case UNJUSTIFIED_OUT:
  case OUT:
    if (a->label != UNJUSTIFIED_OUT){
      for (int i = 0; i < a->numberIsAttacking; i++){
        a->isAttacking[i]->attackedByOut++;
      }
    }
    if (a->label == NOTIN){
      for (int i = 0; i < a->numberIsAttacking; i++){
        a->isAttacking[i]->attackedByNotIn--;
      }
    }
    break;
  case UNDEC:
    for (int i = 0; i < a->numberIsAttacking; i++){
      a->isAttacking[i]->attackedByUndec++;
    }
    if (a->label == NOTIN){
      for (int i = 0; i < a->numberIsAttacking; i++){
        a->isAttacking[i]->attackedByNotIn--;
      }
    }
    break;
  default:
    break;
  }
  li->arguments[li->totalLabeled] = a;

  li->previousLabels[li->totalLabeled] = a->label;

  a->label = newLabel;

  li->totalLabeled++;
  if (newLabel == IN || newLabel == OUT || newLabel == UNDEC){
    li->nFinalLabeled++;
  }

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

    if ((a->label == OUT && changes->previousLabels[changes->totalLabeled] != UNJUSTIFIED_OUT) || a->label == UNJUSTIFIED_OUT){
      for (i = 0; i < a->numberIsAttacking; i++){
        //reverses change that was done in attacked argument when a was labeled OUT
        a->isAttacking[i]->attackedByOut --;
      }
    } else if (a->label == UNDEC){
      for (i = 0; i < a->numberIsAttacking; i++){
        //reverses change that was done in attacked argument when a was labeled UNDEC
        a->isAttacking[i]->attackedByUndec --;
      }
    } else if (a->label == NOTIN){
      for (i = 0; i < a->numberIsAttacking; i++){
        //reverses change that was done in attacked argument when a was labeled NOTIN
        a->isAttacking[i]->attackedByNotIn--;
      }
    }
    //label the argument to the old label again
    a->label = changes->previousLabels[changes->totalLabeled];

    if (a->label == NOTIN){
      for (i = 0; i < a->numberIsAttacking; i++){
        //reverses change that was done when an argument labeled NOTIN was realabeled OUT or UNDEC
        a->isAttacking[i]->attackedByNotIn++;
      }
    }
  }
}

//prints a line with the correct number of leading spaces.
void printInLevel(char * s, int level, bool printTree){
  if (printTree){
    for (int i = 0; i < level; i++){
      printf("   ");
    }
    printf("%s",s);
  }
}

//prints three strings within one line with the correct number of leading spaces.
void printInLevel3(char * s, char * s2, char * s3, int level, bool printTree){
  if (printTree){
    printInLevel(s,level,printTree);
    printf("%s",s2);
    printf("%s",s3);
  }
}











