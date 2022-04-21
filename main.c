/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdio.h>  
#include <stdlib.h> 

#include "solver.h"
#include <time.h>

void printErrorMessage(){
  printf("Please call with <theia -p EE-CO -f PATH>\n");
  exit(1);
}

char *parseArguments(int argc, char *argv[], bool * printTree){
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
    }
  }
  if (ret == NULL){
    printErrorMessage();
  }
  return ret; 
}

int main(int argc, char *argv[]) {
  bool printTree = false; 
  char * input_file = parseArguments(argc, argv,&printTree);
  Graph graph = createGraph(input_file);
  findComplete(graph, printTree); 
  freeGraph(graph); 
  return 0;
}


