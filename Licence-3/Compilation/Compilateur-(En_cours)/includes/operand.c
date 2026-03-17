#include "operand.h"
#include <stdio.h>

type_s bin_opr(type_s x, type_s y, const char* op, bool isFirstParse) {
  if(x == INT_T && y == INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      printf("\t%s bx,ax\n", op);
      printf("\tpush bx\n");
    }
    return INT_T;
  }
  return ERR;
}

type_s div_opr(type_s x, type_s y, bool isFirstParse) {
  if(x==INT_T && y==INT_T) {
    if (!isFirstParse) {
      printf("\tpop ax\n");
      printf("\tpop bx\n");
      printf("\tconst cx,erreur\n");
      printf("\tdiv bx,ax\n");
      printf("\tjmpe cx\n");
      printf("\tpush bx\n");
    }
    return INT_T;
  } else {
    return ERR;
  }
}