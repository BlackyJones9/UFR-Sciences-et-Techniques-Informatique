#include "operand.h"
#include <stdio.h>

type_s inf_bool(type_s x, type_s y, bool isFirstParse) {
  if(x == INT_T && y == INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      int cur_e = etiquette();
      printf("\tconst cx,true_%d\n", cur_e);
      printf("\tsless bx,ax\n");
      printf("\tjmpc cx\n");
      printf("\tconst ax,0\n");
      printf("\tpush ax\n");
      printf("\tconst cx,false_%d\n", cur_e);
      printf("\tjmp cx\n");
      printf(":true_%d\n", cur_e);
      printf("\tconst ax,1\n");
      printf("\tpush ax\n");
      printf(":false_%d\n", cur_e);
    }
    return BOOL_T;
  }
  return ERR;
}

type_s sup_bool(type_s x, type_s y, bool isFirstParse) {
  if(x == INT_T && y == INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      int cur_e = etiquette();
      printf("\tconst cx,true_%d\n", cur_e);
      printf("\tsless ax,bx\n");
      printf("\tjmpc cx\n");
      printf("\tconst ax,0\n");
      printf("\tpush ax\n");
      printf("\tconst cx,false_%d\n", cur_e);
      printf("\tjmp cx\n");
      printf(":true_%d\n", cur_e);
      printf("\tconst ax,1\n");
      printf("\tpush ax\n");
      printf(":false_%d\n", cur_e);
    }
    return BOOL_T;
  }
  return ERR;
}

type_s inf_eq_bool(type_s x, type_s y, bool isFirstParse) {
  if(x == INT_T && y == INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      int cur_e = etiquette();
      printf("\tconst cx,false_%d\n", cur_e);
      printf("\tsless ax,bx\n");
      printf("\tjmpc cx\n");
      printf("\tconst ax,1\n");
      printf("\tpush ax\n");
      printf("\tconst cx,true_%d\n", cur_e);
      printf("\tjmp cx\n");
      printf(":false_%d\n", cur_e);
      printf("\tconst ax,0\n");
      printf("\tpush ax\n");
      printf(":true_%d\n", cur_e);
    }
    return BOOL_T;
  }
  return ERR;
}

type_s sup_eq_bool(type_s x, type_s y, bool isFirstParse) {
  if(x == INT_T && y == INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      int cur_e = etiquette();
      printf("\tconst cx,false_%d\n", cur_e);
      printf("\tsless bx,ax\n");
      printf("\tjmpc cx\n");
      printf("\tconst ax,1\n");
      printf("\tpush ax\n");
      printf("\tconst cx,true_%d\n", cur_e);
      printf("\tjmp cx\n");
      printf(":false_%d\n", cur_e);
      printf("\tconst ax,0\n");
      printf("\tpush ax\n");
      printf(":true_%d\n", cur_e);
    }
    return BOOL_T;
  }
  return ERR;
}

type_s eq_bool(type_s x, type_s y, bool isFirstParse) {
  if(x == INT_T && y == INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      int cur_e = etiquette();
      printf("\tconst cx,true_%d\n", cur_e);
      printf("\tcmp ax,bx\n");
      printf("\tjmpc cx\n");
      printf("\tconst ax,0\n");
      printf("\tpush ax\n");
      printf("\tconst cx,false_%d\n", cur_e);
      printf("\tjmp cx\n");
      printf(":true_%d\n", cur_e);
      printf("\tconst ax,1\n");
      printf("\tpush ax\n");
      printf(":false_%d\n", cur_e);
    }
    return BOOL_T;
  }
  return ERR;
}

type_s diff_bool(type_s x, type_s y, bool isFirstParse) {
  if(x == INT_T && y == INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      int cur_e = etiquette();
      printf("\tconst cx,false_%d\n", cur_e);
      printf("\tcmp ax,bx\n");
      printf("\tjmpc cx\n");
      printf("\tconst ax,1\n");
      printf("\tpush ax\n");
      printf("\tconst cx,true_%d\n", cur_e);
      printf("\tjmp cx\n");
      printf(":false_%d\n", cur_e);
      printf("\tconst ax,0\n");
      printf("\tpush ax\n");
      printf(":true_%d\n", cur_e);
    }
    return BOOL_T;
  }
  return ERR;
}