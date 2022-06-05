/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "heuristic.h"

int OVER_BLANK_IN = 0;
int OVER_BLANK_OUT = 0;
int OVER_BLANK_UNDEC = 0;

int OVER_NOTIN = 0;
int OVER_NOTOUT = 0;
int OVER_NOTUNDEC = 0;

void printStatistics(){
  printf("Splitting arguments was done over:\n");
  printf("BLANK (IN-NOTIN): %d \nBLANK (OUT-NOTOUT): %d \nBLANK (UNDEC-NOTUNDEC): %d \nNOTIN: %d \nNOTOUT: %d \nNOTUNDEC: %d \n"
      , OVER_BLANK_IN, OVER_BLANK_OUT,OVER_BLANK_UNDEC,OVER_NOTIN ,OVER_NOTOUT, OVER_NOTUNDEC);
}

int getArgument(Graph g,ChangeList *changes, Label *labelSplit, Argument *bestArgument, int level, int printTree
                ,char heuristic, SearchCharacteristics SC){
  
  Argument a;
  int currentNumberLabeled;
  double  bestEstimatedTime = DBL_MAX;
  
  for (int i = 0; i < g.numberArguments; i++){
    a = g.arguments[i];
    
    if (a->label == IN || a->label == OUT || a->label == UNDEC){
      continue;
    }
    
    if (a->label == BLANK || a->label == NOTOUT || a->label == NOTUNDEC){
      if (!lookAheadOption(g,changes,a, IN, &bestEstimatedTime, bestArgument, labelSplit,level,printTree, heuristic,SC)){
        return 0;
      }
    } else if (a->label == BLANK || a->label == NOTIN){
      if (!lookAheadOption(g,changes,a, OUT, &bestEstimatedTime, bestArgument, labelSplit,level,printTree, heuristic,SC)){
        return 0;
      }
    } else if (a->label == BLANK){
      if (!lookAheadOption(g,changes,a, UNDEC, &bestEstimatedTime, bestArgument, labelSplit,level,printTree, heuristic,SC)){
        return 0;
      }
    } 
  }

  //for the statistics
  if (*bestArgument != NULL){
    if ((*bestArgument)->label == BLANK){
      if (*labelSplit == IN){
        OVER_BLANK_IN++;
      }
      if (*labelSplit == OUT){
        OVER_BLANK_OUT++;
      }
      if (*labelSplit == UNDEC){
        OVER_BLANK_UNDEC++;
      }
    }
    if ((*bestArgument)->label == NOTIN){
      OVER_NOTIN++;
    }
    if ((*bestArgument)->label == NOTOUT){
      OVER_NOTOUT++;
    }
    if ((*bestArgument)->label == NOTUNDEC){
      OVER_NOTUNDEC++;
    }
  }

  return 1;
  
}

int lookAheadOption(Graph g,ChangeList *changes, Argument a, Label l, double *bestEstimatedTime, Argument *bestArgument
    , Label *labelSplit, int level, int printTree, char heuristic, SearchCharacteristics SC){
  double t1,t2, estimatedTime;
  int currentNumberLabeled = changes->totalLabeled;
  if (!setArgument(a,l,changes,level,false, look_ahead)){
    //This means the argument can not have label l and should get assigned the opposite label
    reverseChanges(changes, currentNumberLabeled);
    if (!setArgument(a,oppositLabel(l),changes,level,printTree, look_ahead)){
      //we found a contradiction!
      reverseChanges(changes, currentNumberLabeled);
      return 0;
    }
    return 1;
  }else{
    t1 = estimateSolvingtime(*changes, heuristic,SC);
    reverseChanges(changes, currentNumberLabeled);
    if (!setArgument(a,oppositLabel(l),changes,level,false, look_ahead)){
      //argument can not get labeled the opposite label of l and therefore gets labeled l 
      reverseChanges(changes, currentNumberLabeled);
      setArgument(a,l,changes,level,printTree, look_ahead); //should not create an error
      return 1;
    }else{
      //both label options were possible
      t2 = estimateSolvingtime(*changes,heuristic,SC);
      reverseChanges(changes, currentNumberLabeled);
    }
  }

  if (heuristic == 'm'){
    estimatedTime = t1 < t2 ? t2 : t1;
  }else {
    estimatedTime = t1 + t2;
  }

  if (estimatedTime <= *bestEstimatedTime){
    *bestEstimatedTime = estimatedTime;
    *bestArgument = a;
    *labelSplit = l;
  }
  return 1;
}


double estimateSolvingtime(ChangeList changes, char heuristic, SearchCharacteristics SC){
  int n_blanks = changes.size - changes.totalLabeled;
  int n_intermediate= changes.size - changes.nFinalLabeled - n_blanks;

  if (heuristic == 'e'){
    return pow(3.0, (double)(n_blanks) / 20) * pow(2.0, (double)(n_intermediate) / 20);
  }else if (heuristic == 's' || heuristic == 'm'){
    return 3 * n_blanks + 2*n_intermediate;
  }else if (heuristic == 'a') {
    //adaptive heuristic
    float averageError = (float)(SC.numberErrors) / SC.numberPropagated;
    float decreaseBLANK = (float)(SC.numberBlankPropagated) / SC.numberPropagated;
    float decreaseIntermediate = (float)(SC.numberIntermediatePropagatied) / SC.numberPropagated;

    return pow(3.0 -3*averageError , (double)(n_blanks)  / (1+decreaseBLANK)   )
          *pow(2.0 -2*averageError , (double)(n_intermediate)    / (1+decreaseIntermediate)   );
    }
  printf("Error, unknown heuristic");
  exit(1);
}

