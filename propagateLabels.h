/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef PROPAGATELABELS_H
#define PROPAGATELABELS_H

#include "argumentList.h" //also includes "graph.h"
#include "heuristic.h"
#include "heap.h"
#include "solver.h"

bool setIn(Argument a, ChangeList *changes, int level, bool printTree,int byRule);
bool setOut(Argument a, ChangeList *changes, int level, bool printTree, bool justified,int byRule);
bool setUndec(Argument a, ChangeList *changes, int level, bool printTree,int byRule);
bool setNotIn(Argument a, ChangeList *changes, int level, bool printTree,int byRule);
bool setNotOut(Argument a, ChangeList *changes, int level, bool printTree, int byRule);

bool rule1(Argument a, ChangeList *changes, int level, bool printTree);
bool rule2(Argument a, ChangeList *changes, int level, bool printTree);
bool rule3(Argument a, ChangeList *changes, int level, bool printTree);
bool rule4(Argument a, ChangeList *changes, int level, bool printTree);
bool rule5(Argument a, ChangeList *changes, int level, bool printTree);
bool rule6(Argument a, ChangeList *changes, int level, bool printTree);
bool rule7(Argument a, ChangeList *changes, int level, bool printTree);
bool rule8(Argument a, ChangeList *changes, int level, bool printTree);
bool rule9(Argument a, ChangeList *changes, int level, bool printTree);
bool rule10(Argument a, ChangeList *changes, int level, bool printTree);
bool rule11(Argument a, ChangeList *changes, int level, bool printTree);
bool rule12(Argument a, ChangeList *changes, int level, bool printTree);
bool rule13(Argument a, ChangeList *changes, int level, bool printTree);

#endif