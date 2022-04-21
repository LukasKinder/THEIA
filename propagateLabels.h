/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#ifndef PROPAGATELABELS_H
#define PROPAGATELABELS_H

bool setArgument(Argument a, Label l, ChangeList *changes, int level, bool printTree, Cause cause);

bool setIn(Argument a, ChangeList *changes, int level, bool printTree,Cause cause);
bool setOut(Argument a, ChangeList *changes, int level, bool printTree,Cause cause);
bool setUndec(Argument a, ChangeList *changes, int level, bool printTree,Cause cause);
bool setNotIn(Argument a, ChangeList *changes, int level, bool printTree,Cause cause);
bool setNotOut(Argument a, ChangeList *changes, int level, bool printTree, Cause cause);
bool setNotUndec(Argument a, ChangeList *changes, int level, bool printTree, Cause cause);

bool rule_1(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_2(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_3(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_4(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_5(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_6(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_7(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_8(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_9(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_10(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_12(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_11(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_13(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_14(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_15(Argument a, ChangeList *changes, int level, bool printTree);
bool rule_16(Argument a, ChangeList *changes, int level, bool printTree);

#endif