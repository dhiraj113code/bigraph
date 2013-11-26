#include <stdio.h>
#include <stdlib.h>

typedef struct vertex_ {
   unsigned node;
   unsigned num;
   unsigned low;
   unsigned color;
} vertex, *Pvertex;
