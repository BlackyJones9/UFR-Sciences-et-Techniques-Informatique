#ifndef BOOLEANS_T
#define BOOLEANS_T

#include "type.h"

extern type_s inf_bool(type_s x, type_s y, bool isFirstParse);
extern type_s sup_bool(type_s x, type_s y, bool isFirstParse);
extern type_s inf_eq_bool(type_s x, type_s y, bool isFirstParse);
extern type_s sup_eq_bool(type_s x, type_s y, bool isFirstParse);
extern type_s eq_bool(type_s x, type_s y, bool isFirstParse);
extern type_s diff_bool(type_s x, type_s y, bool isFirstParse);

#endif