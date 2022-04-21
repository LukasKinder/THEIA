/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

bool setArgument(Argument a, Label l, ChangeList *changes, int level, bool printTree, Cause cause){
  switch (l){
  case IN:
    return setIn(a,changes,level,printTree, cause);
  case OUT:
    return setOut(a,changes,level,printTree, cause);
  case UNDEC:
    return setUndec(a,changes,level,printTree, cause);
  case NOTIN:
    return setNotIn(a,changes,level,printTree, cause);
  case NOTOUT:
    return setNotOut(a,changes,level,printTree, cause);
  case NOTUNDEC:
    return setNotUndec(a,changes,level,printTree, cause);
  }
  exit(1);
}

/*set an argument to IN and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setIn(Argument a, ChangeList *changes, int level, bool printTree, Cause cause){
  if (a->label == IN) return true;

  if (a->label == OUT || a->label == UNDEC || a->label == NOTIN || a->selfAttack){
    //the argument already has a different label
    if (printTree) printContradiction(a,IN,cause, level);
    return false;
  }

  addChange(changes, a, IN,level,printTree,cause);

  if (cause == rule1){
    //all attackers are labeled OUT; no reason to apply rule 8
    return rule_2(a,changes,level,printTree);
  }

  return  rule_2(a,changes,level,printTree) && rule_8(a,changes,level,printTree);
}



/*set an argument to OUT and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setOut(Argument a, ChangeList *changes, int level, bool printTree, Cause cause){
  if (a->label == OUT ) return true;

  if (a->label == IN || a->label == UNDEC || a->label == NOTOUT){
    if (printTree) printContradiction(a,OUT,cause, level);
    return false;
  }

  addChange(changes, a, OUT,level,printTree,cause);

  if (cause == rule2){
    //argument is attacked by an argument labeled IN not point in using rule 7
    return rule_1(a,changes,level,printTree) && rule_3(a,changes,level,printTree) 
      && rule_5(a,changes,level,printTree) && rule_6(a,changes,level,printTree) 
      && rule_13(a,changes,level,printTree) && rule_14(a,changes,level,printTree) 
      && rule_15(a,changes,level,printTree) && rule_15(a,changes,level,printTree);
  }
  return rule_1(a,changes,level,printTree) && rule_3(a,changes,level,printTree) 
    && rule_5(a,changes,level,printTree) && rule_6(a,changes,level,printTree) 
    && rule_7(a,changes,level,printTree) && rule_13(a,changes,level,printTree) 
    && rule_14(a,changes,level,printTree) && rule_15(a,changes,level,printTree) 
    && rule_15(a,changes,level,printTree);
  
}

/*set an argument to UNDEC and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setUndec(Argument a, ChangeList *changes, int level, bool printTree, Cause cause){
  if (a->label == UNDEC) return true;
  
  if (a->label == IN ||a->label == OUT || a->label == NOTUNDEC ){
    //argument already has a different label
    if (printTree) printContradiction(a,UNDEC,cause, level);
    return false;
  }
  addChange(changes, a, UNDEC,level,printTree,cause);
  if (cause ==rule3){
    //rule 9 can be omitted because argument a is already attacked by an argument labeled UNDEC
    return rule_3(a,changes,level,printTree) && rule_13(a,changes,level,printTree) 
    && rule_10(a,changes,level,printTree);
  } 

  return rule_3(a,changes,level,printTree) && rule_9(a,changes,level,printTree) 
    && rule_10(a,changes,level,printTree)&& rule_13(a,changes,level,printTree);

}

/*set an argument to NOTIN and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setNotIn(Argument a, ChangeList *changes, int level, bool printTree, Cause cause){
  switch (a->label){
  case OUT:
  case UNDEC:
  case NOTIN:
    //nothing to do
    return true;
  case IN:
    if (printTree) printContradiction(a,NOTIN,cause, level);
    return false;
  case NOTOUT:
    //argument can not be IN or OUT, therefore is must be UNDEC
    return setUndec(a,changes,level,printTree, cause);
  case NOTUNDEC:
    //argument can not be IN or UNDEC, therefore is must be OUT
    return setOut(a,changes,level,printTree, cause);
  }

  addChange(changes, a, NOTIN,level,printTree,cause);

  return rule_3(a,changes,level,printTree) && rule_5(a,changes,level,printTree)
    && rule_11(a,changes,level,printTree) &&  rule_13(a,changes,level,printTree);
}

/*set an argument to NOTOUT and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setNotOut(Argument a, ChangeList *changes, int level, bool printTree, Cause cause){
  switch (a->label){
  case IN:
  case UNDEC:
  case NOTOUT:
    //nothing to do
    return true;
  case OUT:
    if (printTree) printContradiction(a,NOTOUT,cause, level);
    return false;
  case NOTIN:
    //argument can not be IN or OUT, therefore is must be UNDEC
    return setUndec(a,changes,level,printTree, cause);
  case NOTUNDEC:
    //argument can not be OUT or UNDEC, therefore is must be IN
    return setIn(a,changes,level,printTree, cause);
  }

  addChange(changes, a, NOTOUT,level,printTree,cause);

  if (cause == rule5){
    //all attachers of argument a have the label NOTIN or OUT - rule 10 can be emitted
    return rule_4(a,changes,level,printTree);
  }
  return rule_4(a,changes,level,printTree) && rule_10(a,changes,level,printTree);
}

/*set an argument to NOTUNDEC and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setNotUndec(Argument a, ChangeList *changes, int level, bool printTree, Cause cause){
  switch (a->label)
  {
  case IN:
  case OUT:
  case NOTUNDEC:
    //nothing to do
    return true;
  case UNDEC:
    //contradiction found
    if (printTree) printContradiction(a,NOTUNDEC,cause, level);
    return false;
  case NOTIN:    
    return setOut(a,changes,level,printTree,cause);
  case NOTOUT:
    return setIn(a,changes,level,printTree,cause);
  }

  addChange(changes, a, NOTUNDEC,level,printTree,cause);

  if (cause == rule6){
    //all attachers of argument a have the label NOTUNDEC or OUT - rule 12 can be emitted
    return rule_6(a,changes,level,printTree);
  }
  return rule_6(a,changes,level,printTree) && rule_12(a,changes,level,printTree);
}


/* Should be applied after 'a' got labeled OUT.
Checks if 'a' is attacking an argument 'b' that is now only attacked by arguments 
that are labeled OUT.
If so it labeles 'b' IN.
Returns false if contradiction was found and true otherwise */
bool rule_1(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b;
  for (int i = 0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( (!b->selfAttack || b->label == OUT) 
            && b->attackedByOut == b->numberAttackedBy){
      if (!setIn(b,changes,level,printTree,rule1)){
        return false;
      }
    }
  }
  return true;
}

/* Should be applied if argument 'a' got labeled IN.
It labeles all arguments OUT that are attacked by 'a'.
Returns false if contradiction was found and true otherwise. */
bool rule_2(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i = 0; i < a->numberIsAttacking; i++){
    if (!setOut( a->isAttacking[i], changes,level,printTree,rule2)){
      return false;
    }
  }
  return true;
}


/* Should be applied after 'a' got labeled OUT, UNDEC or NOTIN.
Checks if there is an argument 'b' attacked by 'a' that is now only attacked by arguments
that are labeled OUT, UNDEC or NOTIN
and there is at least one attacking argument that is UNDEC. 
If so it labeles 'b' UNDEC.
Returns false if contradiction was found and true otherwise */
bool rule_3(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b;
  for (int i = 0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->attackedByOut + b->attackedByUndec + b->attackedByNotIn == b->numberAttackedBy
      && b->attackedByUndec > 0){
      if (!setUndec(b,changes,level,printTree,rule3)){
        return false;
      }
    } 
  }
  return true;
}

/*Can be applied if 'a' got labeled NOTOUT or UNDEC
Labeles all arguments that are attacked by 'a' NOTIN
Returns false if contradiction was found and true otherwise */
bool rule_4(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i =0; i < a->numberIsAttacking; i++){
    if ( !setNotIn(a->isAttacking[i],changes,level,printTree,rule4)){
      return false;
    }
  }
  return true;
}

/*Can be applied if 'a' got labeled NOTIN or OUT
Checks if 'a' is attacking some 'b' that is now only attacked by argument labeled NOTIN or OUT.
In this case 'b gets labeled NOTOUT
Returns false if contradiction was found and true otherwise */
bool rule_5(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b;
  for (int i = 0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if (b->attackedByNotIn + b->attackedByOut== b->numberAttackedBy){
      if (!setNotOut(b, changes,level,printTree,rule5)){
        return false;
      }
    }
  } 
  return true;
}

/*Can be applied if 'a' got labeled NOTUNDEC or OUT
Checks if 'a' is attacking some 'b' that is now only attacked by argument labeled NOTUNDEC or OUT.
In this case 'b gets labeled NOTUNDEC
Returns false if contradiction was found and true otherwise */
bool rule_6(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b;
  for (int i = 0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if (b->attackedByNotUndec + b->attackedByOut== b->numberAttackedBy && !b->selfAttack){
      if (!setNotUndec(b, changes,level,printTree,rule6)){
        return false;
      }
    }
  } 
  return true;
}

/*Can be applied if 'a' got labeled OUT
Checks if 'a' is only attacked by arguments labeled OUT, UNDEC or NOTIN except of one argument 'b'
In this case 'b gets labeled IN. 
Returns false if contradiction was found and true otherwise.*/
bool rule_7(Argument a, ChangeList *changes, int level, bool printTree){
  if (a->numberAttackedBy - a->attackedByOut - a->attackedByUndec - a->attackedByNotIn == 1){
    Argument b;
    for (int i  = 0; i < a->numberAttackedBy; i++){
      b = a->attackedBy[i]; 
      if (b->label != OUT && b->label != UNDEC && b->label != NOTIN){
        return setIn(b,changes,level,printTree,rule7);
      }
    }
  }
  return true;
}

/* Should be applied if argument 'a' got labeled IN.
It labeles all arguments OUT that are attacking 'a'.
Returns false if contradiction was found and true otherwise. */
bool rule_8(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i = 0; i < a->numberAttackedBy; i++){
    if (!setOut( a->attackedBy[i], changes,level,printTree,rule8)){
      return false;
    }
  }
  return true;
}


/*Can be applied if 'a' got labeled UNDEC
Checks if 'a' is only attacked by arguments that are labeled OUT except for 
some argument 'b'.
In this case 'b gets labeled UNDEC
Returns false if contradiction was found and true otherwise */
bool rule_9(Argument a, ChangeList *changes, int level, bool printTree){
  if (a->selfAttack){
    //a is already attacked by an Argument that has the labeled UNDEC, namely itself
    return true;
  }

  Argument b;
  if (a->attackedByOut == a->numberAttackedBy -1){
    for (int i = 0; i < a->numberAttackedBy; i++){
      b = a->attackedBy[i];
      if (b->label != OUT){
        return setUndec(b, changes,level,printTree,rule9);
      }
    }
  }
  return true;
}

/*Can be applied if 'a' got labeled NOTOUT or UNDEC
Labeles all arguments that are attacking 'a' NOTIN
Returns false if contradiction was found and true otherwise */
bool rule_10(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i  =0; i < a->numberAttackedBy; i++){
    if ( !setNotIn(a->attackedBy[i],changes,level,printTree,rule10)){
      return false;
    }
  }
  return true;
}


/*Can be applied if 'a' got labeled NOTIN
Checks if 'a' is only attacked by arguments that are labeled OUT except for 
some argument 'b'.
In this case 'b gets labeled NOTOUT
Returns false if contradiction was found and true otherwise */
bool rule_11(Argument a, ChangeList *changes, int level, bool printTree){
  if (a->selfAttack){
    //a is already attacked by an argument that is not labeled Out, namely itself. 
    return true;
  }
  Argument b;
  if (a->attackedByOut == a->numberAttackedBy -1){
    //there is only one argument that is attackign 'a' and is not labeled OUT. 
    for (int i  =0; i < a->numberAttackedBy; i++){
      b = a->attackedBy[i];
      if (b->label != OUT){
        return setNotOut(b,changes,level,printTree,rule11);
      }
    }
  }
  //a in not attacked by arguments labeled OUT except one
  return true;
}

/*Can be applied if 'a' got labeled NOTUNDEC
Checks if 'a' is only attacked by arguments that are labeled OUT except for 
some argument 'b'.
In this case 'b gets labeled NOTUNDEC
Returns false if contradiction was found and true otherwise */
bool rule_12(Argument a, ChangeList *changes, int level, bool printTree){
  if (a->selfAttack){
    //a is already attacked by an argument that is not labeled Out, namely itself. 
    return true;
  }
  Argument b;
  if (a->attackedByOut == a->numberAttackedBy -1){
    //there is only one argument that is attackign 'a' and is not labeled OUT. 
    for (int i  =0; i < a->numberAttackedBy; i++){
      b = a->attackedBy[i];
      if (b->label != OUT){
        return setNotUndec(b,changes,level,printTree,rule12);
      }
    }
  }
  //a in not attacked by arguments labeled OUT except one
  return true;
}


/*Can be applied if 'a' got labeled OUT, UNDEC or NOTIN
Checks if 'a' is attacking an argument 'b' that has the label OUT.
In this case it checks if, except of some argument c, all attackers of b
are labeled OUT, UNDEC or NOTIN.
In this case c gets labeled IN.
Returns false if contradiction was found and true otherwise */
bool rule_13(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b,c;
  for (int i  =0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->label == OUT
        && b->numberAttackedBy - b->attackedByOut - b->attackedByUndec - b->attackedByNotIn ==1) {
      //rule can be applied 
      for (int j = 0; j < b->numberAttackedBy; j++){
        c = b->attackedBy[j];
        if (c->label != OUT && c->label != UNDEC && c->label != NOTIN){
          //we found the right c
          if (!setIn(c,changes,level,printTree,rule13)){
            return false;
          }
          break;
        }
      }
    }
  }
  return true;
}

/*Can be applied if 'a' got labeled OUT
Checks if 'a' is attacking an argument 'b' that has the label UNDEC.
In this case it checks if, except of some argument c, all attackers of b
are labeled OUT.
In this case c gets labeled UNDEC.
Returns false if contradiction was found and true otherwise */
bool rule_14(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b,c;
  for (int i  =0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->label == UNDEC && !b->selfAttack && b->numberAttackedBy - b->attackedByOut ==1) {
      //rule can be applied 
      for (int j = 0; j < b->numberAttackedBy; j++){
        c = b->attackedBy[j];
        if (c->label != OUT){
          //we found the right c
          if (!setUndec(c,changes,level,printTree,rule14)){
            return false;
          }
          break;
        }
      }
    }
  }
  return true;
}

/*Can be applied if 'a' got labeled OUT
Checks if 'a' is attacking an argument 'b' that has the label NOTIN.
In this case it checks if, except of some argument c, all attackers of b
are labeled OUT.
In this case c gets labeled NOTOUT.
Returns false if contradiction was found and true otherwise */
bool rule_15(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b,c;
  for (int i  =0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->label == NOTIN && b->numberAttackedBy - b->attackedByOut ==1) {
      //rule can be applied 
      for (int j = 0; j < b->numberAttackedBy; j++){
        c = b->attackedBy[j];
        if (c->label != OUT){
          //we found the right c
          if (!setNotOut(c,changes,level,printTree,rule15)){
            return false;
          }
          break;
        }
      }
    }
  }
  return true;
}


/*Can be applied if 'a' got labeled OUT
Checks if 'a' is attacking an argument 'b' that has the label NOTUNDEC.
In this case it checks if, except of some argument c, all attackers of b
are labeled OUT.
In this case c gets labeled NOTUNDEC.
Returns false if contradiction was found and true otherwise */
bool rule_16(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b,c;
  for (int i  =0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->label == NOTUNDEC && b->numberAttackedBy - b->attackedByOut ==1) {
      //rule can be applied 
      for (int j = 0; j < b->numberAttackedBy; j++){
        c = b->attackedBy[j];
        if (c->label != OUT){
          //we found the right c
          if (!setNotUndec(c,changes,level,printTree,rule16)){
            return false;
          }
          break;
        }
      }
    }
  }
  return true;
}













