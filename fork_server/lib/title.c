#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "title.h"

void set_empty_title(title *t){
  strcpy(t->title1,"");
  strcpy(t->title2,"");
  t->pick = -1;
}
