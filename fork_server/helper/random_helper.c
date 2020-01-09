#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "random_helper.h"

int rand_flag = 0;

int random_title_id(){
  return random_from_0_to_max(20); // 3 is max of titles
}

int random_from_0_to_max(int max){
  if(!rand_flag){
    srand(time(NULL));
    rand_flag = 1;
  }
  int r = rand();
  int x = max+1;
  return r%x;
}
