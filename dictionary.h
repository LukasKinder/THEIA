/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

#include "graph.h"

/*A hastable containing arguments. The hash-key of an argument is computed using the
argument name. This hash-table enables to quickly find an argument given its name */
typedef struct HashTable {
  int numberRows;
  int *sizeRows;
  int *numberItems;
  Argument **table;
}HashTable;

int stringToInt(char * name);
Argument nameToArgument(char *name, HashTable dic);
void unknownNameError(char *name);
void enterArgument(Argument a, HashTable dic);
void freeDictionary(HashTable tb);
HashTable createDictionary(int numberRows);




#endif