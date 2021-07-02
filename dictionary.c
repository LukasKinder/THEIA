/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

HashTable createDictionary(int numberRows){
  HashTable tb;
  tb.numberRows = numberRows;
  tb.numberItems = malloc(sizeof(int) * numberRows);
  tb.sizeRows =  malloc(sizeof(int) * numberRows);
  tb.table = malloc(sizeof(Argument *) *  numberRows);
  for (int i =0; i < numberRows; i++){
    tb.numberItems[i] = 0;
    tb.sizeRows[i] = 1;
    tb.table[i] = malloc(sizeof(Argument ));
  }
  return tb;
}
void freeDictionary(HashTable tb){
  for (int i =0; i < tb.numberRows; i++){

    free(tb.table[i]); 
  }
  free(tb.numberItems);
  free(tb.sizeRows);
  free(tb.table);
}

//uses argument name as hash key
void enterArgument(Argument a, HashTable dic){
  int key = stringToInt(a->name) % dic.numberRows;

  if (dic.sizeRows[key] = dic.numberItems[key]){
    dic.sizeRows[key] *=2;
    dic.table[key] = realloc(dic.table[key], sizeof(Argument) * dic.sizeRows[key]);
  }
  dic.table[key][dic.numberItems[key]] = a;
  dic.numberItems[key]++;
}

void unknownNameError(char *name){
  printf("ERROR: Name '%s' is unknown in dictionary\n", name);
  exit(1); 
}

/* Find the argument given the name of the argument by first computing
the corresponding hash-key of the argument-name */
Argument nameToArgument(char *name, HashTable dic){
  int key = stringToInt(name) % dic.numberRows;

  Argument a;
  for (int i =0; i < dic.numberItems[key]; i++){
    a = dic.table[key][i];
    if (strcmp(name,a->name) == 0){
      return a;
    }
  }
  unknownNameError(name);
}

//creates hash-key using the name of the argument
int stringToInt(char * name){
  int key = 0, len = strlen( name);
  for (int i =0; i < len; i++){
    key += (int) (name[i]);
  }
  return key;
}
