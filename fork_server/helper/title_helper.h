#ifndef TITLE_HELPER_H
#define TITLE_HELPER_H

#include "../lib/title.h"
#include "../lib/protocol.h"

char *get_title_fname(title);   /*get title that picked by client*/
char *get_title_from_id(int);   /*return title base on id*/
void get_question_from_title(char *, question *,int);

#endif
