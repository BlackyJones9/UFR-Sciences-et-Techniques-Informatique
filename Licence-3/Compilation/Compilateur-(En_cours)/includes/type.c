#include "type.h"

int etiquette(void) {
  static int k = 0;
  k++;
  return k;
}