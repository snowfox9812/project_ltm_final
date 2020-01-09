#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "answer_helper.h"
#include "title_helper.h"

int find_right_answer(protocol *p){
  question q = p->p_question;
  int id = q.question_id;
  char f_name[256] = "title/";
  strcat(f_name,get_title_fname(p->p_title));
  FILE *f;
  int right_answer = 0;
  f = fopen(f_name,"r");
  if(f){
    int i = 0;
    char s[256] = "";
    for(i = 0; i <= id; i++){
      memset(s,'\0',strlen(s));
      fscanf(f,"%[^\n]",s);
      fgetc(f);
    }
    right_answer = s[strlen(s)-1] - '0';
  }
  fclose(f);
  return right_answer;
}

int get_remain_score(answer a, int right_answer){
  int remain_score = 0;
  switch (right_answer) {
    case 1:
      remain_score = a.bet1;
      break;
    case 2:
      remain_score = a.bet2;
      break;
    case 3:
      remain_score = a.bet3;
      break;
    case 4:
      remain_score = a.bet4;
      break;
  }
  return remain_score;
}

char *get_right_answer_message(int right_answer){
  char *message;
  message = (char *)malloc(256);
  switch (right_answer) {
    case 1:
      strcpy(message,"Right answer is 1");
      break;
    case 2:
      strcpy(message,"Right answer is 2");
      break;
    case 3:
      strcpy(message,"Right answer is 3");
      break;
    case 4:
      strcpy(message,"Right answer is 4");
      break;
  }
  return message;
}
