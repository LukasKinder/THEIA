/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdio.h>  
#include <stdlib.h> 

#include "graph.h"
#include "solver.h"
#include <time.h>


int main(int argc, char *argv[]) {

  if (argc < 2){
    printf("ERROR: no inout file given\n");
    exit(1);
  }

  bool printTree = false; 
  if (argc > 2){
    printTree =  (argv[2][0] - '0') == 1 ? true : false;
  }

  float averageAttacks;

  Graph graph = createGraph(argv[1], &averageAttacks);

  if (averageAttacks < 50){
    findComplete(graph, printTree, true); 
    printf("(new version)");
  }else{
    findComplete(graph, printTree, false); 
    printf("(old version)");
  }
  freeGraph(graph); 

  return 0;
}
