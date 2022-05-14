/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdio.h>  
#include <stdlib.h> 

#include "solver.h"
#include <time.h>

void printErrorMessage(){
  printf("Please call with <theia -p EE-CO -f PATH -h HEURISTIC>\n");
  exit(1);
}

char *parseArguments(int argc, char *argv[], bool * printTree, char * heuristic){
  char *ret = NULL;
  for (int i =1; i < argc; i++){
    if (strcmp(argv[i], "-p") == 0){
      if ( strcmp(argv[i+1], "EE-CO") != 0 ){
        printf("ERROR: The only task implemented is EE-CO\n");
        printErrorMessage();
      }
      i+=1; 
    }else if (strcmp(argv[i], "-f") == 0){
      ret = argv[i+1];
      i+=1; 
    }else if (strcmp(argv[i], "-v") == 0){
      *printTree = true;
    }else if (strcmp(argv[i], "-h") == 0){
      if (strcmp(argv[i+1], "min") == 0){
        *heuristic = 'm';
      }else if (strcmp(argv[i+1], "sum") == 0){
        *heuristic = 's';
      }else if (strcmp(argv[i+1], "exp") == 0){
        *heuristic = 'e';
      }else {
        printf("ERROR: Unknown heuristic\n");
        printErrorMessage();
      }
    } 
  }
  if (ret == NULL){
    printErrorMessage();
  }
  return ret; 
}

int main(int argc, char *argv[]) {
  bool printTree = false; 
  char heuristic = 'e';
  char * input_file = parseArguments(argc, argv,&printTree,&heuristic);
  Graph graph = createGraph(input_file);
  findComplete(graph, printTree, heuristic); 
  freeGraph(graph); 
  return 0;
}


