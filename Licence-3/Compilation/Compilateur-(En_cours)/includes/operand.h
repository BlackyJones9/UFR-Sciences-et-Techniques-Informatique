#ifndef OPERAND_T
#define OPERAND_T

#include "type.h"

extern type_s bin_opr(type_s x, type_s y, const char* op, bool isFirstParse);
extern type_s div_opr(type_s x, type_s y, bool isFirstParse);
// extern type_s add_opr(type_s x, type_s y);
// extern type_s sub_opr(type_s x, type_s y);
// extern type_s mul_opr(type_s x, type_s y);

#endif 