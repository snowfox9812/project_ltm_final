#ifndef QUESTION_H
#define QUESTION_H

typedef struct question_t{
  char title[256];
  int question_id;
  char question[256];
  char answer1[256];
  char answer2[256];
  char answer3[256];
  char answer4[256];
  int number_answers;
}question;

void set_empty_question(question *);

#endif
