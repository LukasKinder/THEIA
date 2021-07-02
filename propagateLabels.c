/*Lukas Kinder (s3686566), lukas.m.kinder@gmail.com, Bachelor Project 2021 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

/*set an argument to IN and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setIn(Argument a, ChangeList *changes, int level, bool printTree, int byRule){
  if (a->label == IN) return true;

  if (a->label != BLANK && a->label != NOTOUT){
    //the argument already has a different label or is attacing itself
    printInLevel3( a->name, " = IN is contradiction because it has a different label already."," ",level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    return false;
  }

  printInLevel3(a->name, " = IN"," " ,level, printTree );
  if (printTree) printf("  (By Rule %d)\n", byRule);
  addChange(changes, a, IN);

  if (! rule1(a,changes,level,printTree)){
    return false;
  }
  if ( byRule !=3 && !rule2(a,changes,level,printTree)){
    return false;
  }
  return true;
}

/*set an argument to UNDEC and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setUndec(Argument a, ChangeList *changes, int level, bool printTree, int byRule){
  if (a->label == UNDEC) return true;
  
  if (a->label != BLANK && a->label != NOTOUT && a->label != NOTIN){
    //argument already has a different label
    printInLevel3( a->name, " = UNDEC is contradiction because it has a different label already."," ",level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    return false;
  }
  printInLevel3(a->name, " = UNDEC"," " ,level, printTree );
  if (printTree) printf("  (By Rule %d)\n", byRule);
  int i;
  addChange(changes, a, UNDEC);

  switch (byRule)
  {
  case 0:
    return rule5(a,changes,level,printTree) && rule6(a,changes,level,printTree)
      && rule11(a,changes,level,printTree);
  case -9:
    return rule6(a,changes,level,printTree) && rule9(a,changes,level,printTree) 
      && rule10(a,changes,level,printTree);
  case 5:
    return rule5(a,changes,level,printTree) && rule9(a,changes,level,printTree)
      && rule11(a,changes,level,printTree);
  default:
    return rule5(a,changes,level,printTree) && rule6(a,changes,level,printTree)
      && rule9(a,changes,level,printTree) && rule10(a,changes,level,printTree)
      && rule11(a,changes,level,printTree);
  }
  printf("should not happen");
  exit(1);
}

/*set an argument to OUT and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setOut(Argument a, ChangeList *changes, int level, bool printTree, bool justified, int byRule){
  if (a->label == OUT ) return true;
  if (a->label == UNJUSTIFIED_OUT){
    if (justified){
      printInLevel3(a->name, " = OUT"," " ,level, printTree );
      if (printTree) printf("  (By Rule %d)\n", byRule);
      addChange(changes, a, OUT);
    }
    return true;
  }

  if (a->label != BLANK && a->label != NOTIN){
    printInLevel3( a->name, " = OUT is contradiction because it has a different label already."," ",level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    return false;
  }

  if (justified){
    printInLevel3(a->name, " = OUT"," " ,level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    addChange(changes, a, OUT);
  }else{
    printInLevel3(a->name, " = UNJUSTIFIED_OUT"," " ,level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    addChange(changes, a, UNJUSTIFIED_OUT);
  }

  switch (byRule){
  case 1:
    return rule3(a,changes,level,printTree) 
      && rule5(a,changes,level,printTree) && rule7(a,changes,level,printTree)
      && rule11(a,changes,level,printTree) && rule12(a,changes,level,printTree);
  default:
    return rule3(a,changes,level,printTree) && rule4(a,changes,level,printTree) 
      && rule5(a,changes,level,printTree) && rule7(a,changes,level,printTree)
      && rule11(a,changes,level,printTree) && rule12(a,changes,level,printTree)
      && rule13(a,changes,level,printTree);
  }
  printf("should not happen");
  exit(1);

}

/*set an argument to NOTIN and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setNotIn(Argument a, ChangeList *changes, int level, bool printTree, int byRule){
  if (a->label == UNJUSTIFIED_OUT || a->label == OUT || a->label == UNDEC || a->label == NOTIN){
    return true;
  }else if (a->label == IN){
    printInLevel3( a->name, " = NOTIN is contradiction because it is labeled IN."," ",level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    return false;
  }else if (a->label == NOTOUT){
    //argument can not be IN or OUT, therefore is must be UNDEC
    printInLevel3(a->name, " = NOTIN"," " ,level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    return setUndec(a,changes,level,printTree, 0);
  }

  printInLevel3(a->name, " = NOTIN"," " ,level, printTree );
  if (printTree) printf("  (By Rule %d)\n", byRule);
  addChange(changes, a, NOTIN);

  return rule5(a,changes,level,printTree) && rule7(a,changes,level,printTree) 
    && rule8(a,changes,level,printTree) && rule11(a,changes,level,printTree);
}

/*set an argument to OUT and recursively label new arguments if possible
returns true if no contradiction was found, false otherwise*/
bool setNotOut(Argument a, ChangeList *changes, int level, bool printTree, int byRule){
  if (a->label == IN || a->label == UNDEC || a->label == NOTOUT){
    return true;
  }else if (a->label == OUT || a->label == UNJUSTIFIED_OUT){
    printInLevel3( a->name, " = NOTOUT is contradiction because it is labeled OUT."," ",level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    return false;
  }else if (a->label == NOTIN){
    printInLevel3(a->name, " = NOTOUT"," " ,level, printTree );
    if (printTree) printf("  (By Rule %d)\n", byRule);
    return setUndec(a,changes,level,printTree,-9);
  }

  printInLevel3(a->name, " = NOTOUT"," " ,level, printTree );
  if (printTree) printf("  (By Rule %d)\n", byRule);
  addChange(changes, a, NOTOUT);

  switch (byRule){
  case 7:
    return rule9(a,changes,level,printTree);
  default:
    return rule9(a,changes,level,printTree) && rule10(a,changes,level,printTree);
  }
  printf("should not happen");
  exit(1);
}
/* Should be applied if argument 'a' got labeled IN.
It labeles all arguments OUT that are attacked by 'a'.
Returns false if contradiction was found and true otherwise. */
bool rule1(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i = 0; i < a->numberIsAttacking; i++){
    if (!setOut( a->isAttacking[i], changes,level,printTree,true,1)){
      return false;
    }
  }
  return true;
}

/* Should be applied if argument 'a' got labeled IN.
It labeles all arguments OUT that are attacking 'a'.
Returns false if contradiction was found and true otherwise. */
bool rule2(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i = 0; i < a->numberAttackedBy; i++){
    if (!setOut( a->attackedBy[i], changes,level,printTree,false,2)){
      return false;
    }
  }
  return true;
}

/* Should be applied after 'a' got labeled OUT.
Checks if 'a' is attacking an argument 'b' that is now only attacked by arguments 
that are labeled OUT.
If so it labeles 'b' IN.
Returns false if contradiction was found and true otherwise */
bool rule3(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b;
  for (int i = 0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( (!b->selfAttack || b->label == OUT || b->label == UNJUSTIFIED_OUT ) 
            && b->attackedByOut == b->numberAttackedBy){
      if (!setIn(b,changes,level,printTree,3)){
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
bool rule4(Argument a, ChangeList *changes, int level, bool printTree){
  if (a->numberAttackedBy - a->attackedByOut - a->attackedByUndec - a->attackedByNotIn == 1){
    Argument b;
    for (int i  = 0; i < a->numberAttackedBy; i++){
      b = a->attackedBy[i]; 
      if (b->label != OUT && b->label != UNJUSTIFIED_OUT && b->label != UNDEC && b->label != NOTIN){
        if ( !setIn(b,changes,level,printTree,4)){
          return false;
        }
      }
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
bool rule5(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b;
  for (int i = 0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->attackedByOut + b->attackedByUndec + b->attackedByNotIn == b->numberAttackedBy
      && b->attackedByUndec > 0){
      if (!setUndec(b,changes,level,printTree,5)){
        return false;
      }
    } 
  }
  return true;
}

/*Can be applied if 'a' got labeled UNDEC
Checks if 'a' is only attacked by arguments that are labeled OUT except for 
some argument 'b'.
In this case 'b gets labeled UNDEC
Returns false if contradiction was found and true otherwise */
bool rule6(Argument a, ChangeList *changes, int level, bool printTree){
  if (a->selfAttack){
    //a is already attacked by an Argument that has the labeled UNDEC, namely itself
    return true;
  }

  Argument b;
  if (a->attackedByOut == a->numberAttackedBy -1){
    for (int i = 0; i < a->numberAttackedBy; i++){
      b = a->attackedBy[i];
      if (b->label != OUT && b->label != UNJUSTIFIED_OUT){
        return setUndec(b, changes,level,printTree,6);
      }
    }
  }
  return true;
}

/*Can be applied if 'a' got labeled NOTIN or OUT
Checks if 'a' is attacking some 'b' that is now only attacked by argument labeled NOTIN or OUT.
In this case 'b gets labeled NOTOUT
Returns false if contradiction was found and true otherwise */
bool rule7(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b;
  for (int i = 0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if (b->attackedByNotIn + b->attackedByOut== b->numberAttackedBy){
      if (!setNotOut(b, changes,level,printTree,7)){
        return false;
      }
    }
  } 
  return true;
}

/*Can be applied if 'a' got labeled NOTIN
Checks if 'a' is only attacked by arguments that are labeled OUT except for 
some argument 'b'.
In this case 'b gets labeled NOTOUT
Returns false if contradiction was found and true otherwise */
bool rule8(Argument a, ChangeList *changes, int level, bool printTree){
  if (a->selfAttack){
    //a is already attacked by an argument that is not labeled Out, namely itself. 
    return true;
  }
  Argument b;
  if (a->attackedByOut == a->numberAttackedBy -1){
    //there is only one argument that is attackign 'a' and is not labeled OUT. 
    for (int i  =0; i < a->numberAttackedBy; i++){
      b = a->attackedBy[i];
      if (b->label != OUT && b->label != UNJUSTIFIED_OUT){
        return setNotOut(b,changes,level,printTree,8);
      }
    }
  }
  //a in not attacked by arguments labeled OUT except one
  return true;
}

/*Can be applied if 'a' got labeled NOTOUT or UNDEC
Labeles all arguments that are attacked by 'a' NOTIN
Returns false if contradiction was found and true otherwise */
bool rule9(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i  =0; i < a->numberIsAttacking; i++){
    if ( !setNotIn(a->isAttacking[i],changes,level,printTree,9)){
      return false;
    }
  }
  return true;
}

/*Can be applied if 'a' got labeled NOTOUT or UNDEC
Labeles all arguments that are attacking 'a' NOTIN
Returns false if contradiction was found and true otherwise */
bool rule10(Argument a, ChangeList *changes, int level, bool printTree){
  for (int i  =0; i < a->numberAttackedBy; i++){
    if ( !setNotIn(a->attackedBy[i],changes,level,printTree,10)){
      return false;
    }
  }
  return true;
}

/*Can be applied if 'a' got labeled OUT, UNDEC or NOTIN
Checks if 'a' is attacking an argument 'b' that has the label OUT.
In this case it checks if, except of some argument c, all attackers of b
are labeled OUT, UNDEC or NOTIN.
In this case c gets labeled IN.
Returns false if contradiction was found and true otherwise */
bool rule11(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b,c;
  for (int i  =0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->label == UNJUSTIFIED_OUT
        && b->numberAttackedBy - b->attackedByOut - b->attackedByUndec - b->attackedByNotIn ==1) {
      //rule can be applied 
      for (int j = 0; j < b->numberAttackedBy; j++){
        c = b->attackedBy[j];
        if (c->label != OUT && c->label != UNJUSTIFIED_OUT && c->label != UNDEC && c->label != NOTIN){
          //we found the right c
          if (!setIn(c,changes,level,printTree,11)){
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
bool rule12(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b,c;
  for (int i  =0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->label == UNDEC && !b->selfAttack && b->numberAttackedBy - b->attackedByOut ==1) {
      //rule can be applied 
      for (int j = 0; j < b->numberAttackedBy; j++){
        c = b->attackedBy[j];
        if (c->label != OUT && c->label != UNJUSTIFIED_OUT){
          //we found the right c
          if (!setUndec(c,changes,level,printTree,12 )){
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
bool rule13(Argument a, ChangeList *changes, int level, bool printTree){
  Argument b,c;
  for (int i  =0; i < a->numberIsAttacking; i++){
    b = a->isAttacking[i];
    if ( b->label == NOTIN && b->numberAttackedBy - b->attackedByOut ==1) {
      //rule can be applied 
      for (int j = 0; j < b->numberAttackedBy; j++){
        c = b->attackedBy[j];
        if (c->label != OUT && c->label != UNJUSTIFIED_OUT){
          //we found the right c
          if (!setNotOut(c,changes,level,printTree,13 )){
            return false;
          }
          break;
        }
      }
    }
  }
  return true;
}












