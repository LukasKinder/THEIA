/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

//Creates a new argument without attack relations
//leaves  heuristic uninitialized!
Argument newArgument(char *name){
  Argument a = malloc(sizeof(Node));
  a->isAttacking = NULL;
  a->attackedBy = NULL;
  a->label = BLANK;
  a->name = name;
  a->numberIsAttacking = 0;
  a->numberAttackedBy = 0;
  a->selfAttack = false;
  a->sizeIsAttackingArray = 0;
  a->sizeAttackedByArray = 0;
  a->attackedByOut = 0;
  a->attackedByUndec = 0;
  a->attackedByNotIn = 0;
  a->attackedByNotUndec = 0;
  return a;
}

void freeArgument(Argument a){
  free(a->name);
  free(a->attackedBy);
  free(a->isAttacking);
  free(a);
}

//adds the information to argument a that it is attacking argument b
void addIsAttacking(Argument a, Argument b){
  if (a->sizeIsAttackingArray == 0){
    a->sizeIsAttackingArray = 1;
    a->isAttacking = malloc(sizeof(Node));
  }else if (a->numberIsAttacking == a->sizeIsAttackingArray){
    a->sizeIsAttackingArray *=2;
    a->isAttacking = realloc(a->isAttacking, sizeof(Node) * a->sizeIsAttackingArray  );
  }
  a->isAttacking[a->numberIsAttacking] = b;
  a->numberIsAttacking++;
}

//adds the information to argument a that it is attacked by argument b
void addAttackedBy(Argument a, Argument b){
  if (a->sizeAttackedByArray == 0){
    a->sizeAttackedByArray = 1;
    a->attackedBy = malloc(sizeof(Node));
  }else if (a->numberAttackedBy == a->sizeAttackedByArray){
    a->sizeAttackedByArray *=2;
    a->attackedBy = realloc(a->attackedBy,sizeof(Node) * a->sizeAttackedByArray  );
  }
  a->attackedBy[a->numberAttackedBy] = b;
  a->numberAttackedBy++;
}

//adds the information to a that it is attacking itself 
void addSelfAttack(Argument a){
  a->selfAttack = true;
}

/*-Scans the graph that is represented in the trivial Graph format,
assumes arguments are numbers from 1 onwards,
Ending of relations must be indicated as a "-1"
*/
Graph createGraph(char * filename){
  FILE * fp;
  char c[1000];
  char * line = NULL;
  size_t len = 0;
  size_t read;
  fp = fopen(filename, "r");
  if (fp == NULL){
    printf("ERRROR: Could not open file '%s'\n",filename);
    exit(1);
  }

  char *name1, *name2;
  int lenArgumentArray = 2;
  Graph g;
  g.numberArguments = 0;
  g.arguments = malloc(sizeof(Argument) * lenArgumentArray);
  Argument a,b;
  HashTable dic = createDictionary(1000);

  while ((read = getline(&line, &len, fp)) != -1){
    if (line[0] == '#'){
      break;
    }
    removeLinebreak(line, (int)read);
    name1  = malloc(sizeof(char) * ((int)read ));
    strcpy(name1, line);

    g.numberArguments++;
    if (g.numberArguments > lenArgumentArray){
      lenArgumentArray *=2;
      g.arguments = realloc(g.arguments,sizeof(Argument)*lenArgumentArray);
    }
    a = newArgument(name1);
    enterArgument(a,dic);
    g.arguments[g.numberArguments-1] = a;
  }

  while ((read = getline(&line, &len, fp)) != -1){
    if (line[0] == '\n'){
      break;
    }
    removeLinebreak(line, (int)read);
    name1  = strtok(line," ");
    name2 = strtok(NULL," ");
    a = nameToArgument(name1,dic);
    b = nameToArgument(name2,dic);


    if (strcmp(name1,name2) == 0){

      addSelfAttack(a);
    }else{
      addIsAttacking(a, b);
      addAttackedBy(b, a);
    }
  }
  
  if (line) {
    free(line);
  }
  fclose(fp);
  freeDictionary(dic);
  return g;
}

void freeGraph(Graph g){
  for (int i =0; i < g.numberArguments; i++){
    freeArgument(g.arguments[i]);
  }
  free(g.arguments);
}

void printLabel(Label l){
  switch (l)
  {
  case IN:
    printf("IN");
    break;
  case OUT:
    printf("OUT");
    break;
  case UNDEC:
    printf("UNDEC");
    break;
  case BLANK:
    printf("BLANK");
    break;
  case NOTIN:
    printf("NOTIN");
    break;
  case NOTOUT:
    printf("NOTOUT");
    break;
  case NOTUNDEC:
    printf("NOTUNDEC");
    break;
  }
}

//prints a list of all arguments in the graph that have the label IN
void printInArguments(Graph graph){
  Argument a;
  bool first = true;
  printf("[");
  for (int i =0; i < graph.numberArguments; i++){
    a = graph.arguments[i];
    if (a->label == IN){
      if (first){
        printf("%s",a->name);
        first = false;
      }else{
        printf(",%s",a->name);
      }
    }
  }
  printf("]\n");
}

/*replaces linebreak in sting with '\0' */
void removeLinebreak(char *str, int end) {
  for (int i =0; i < end; i++){
    if (str[i] == '\n'){
      str[i] = '\0';
    }
    if (str[i] == '\r'){
      str[i] = '\0';
    }
  }
}

Label oppositLabel(Label l){
  switch (l){
  case IN:
    return NOTIN;
  case OUT:
    return NOTOUT;
  case UNDEC:
    return NOTUNDEC;
  case NOTIN:
    return IN;
  case NOTOUT:
    return OUT;
  case NOTUNDEC:
    return UNDEC;
  }
  exit(1);
}

void printState(Graph g){
  Argument b;
  for (int j = 0; j < g.numberArguments; j++){
    b = g.arguments[j];
    printf("%s",b->name);
    printf(" ");
    printLabel(b->label);
  }
}