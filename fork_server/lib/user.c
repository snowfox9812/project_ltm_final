#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void set_empty_user(user *u){
  strcpy(u->user_id,"");
  strcpy(u->password,"");
}
