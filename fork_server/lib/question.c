#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "question.h"

void set_empty_question(question *q){
  strcpy(q->title,"");
  q->question_id = 0;
  strcpy(q->question,"");
  strcpy(q->answer1,"");
  strcpy(q->answer2,"");
  strcpy(q->answer3,"");
  strcpy(q->answer4,"");
  q->number_answers = 0;
}
