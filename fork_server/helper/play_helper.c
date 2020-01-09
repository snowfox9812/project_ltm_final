#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "play_helper.h"
#include "random_helper.h"
#include "history_helper.h"
#include "title_helper.h"
#include "answer_helper.h"

void send_titles(protocol *p, int remain_score){
  p->p_state = PLAYING;
  memset(p->p_message,'\0',sizeof(p->p_message));
  int id;
  do{
    id = random_title_id();
    printf("%d\n", id);
  } while(match_history(p->p_user_info,id));
  write_history(p->p_user_info,id);
  strcpy(p->p_title.title1,get_title_from_id(id));
  do{
    id = random_title_id();
    printf("%d\n", id);
  } while(match_history(p->p_user_info,id));
  write_history(p->p_user_info,id);
  strcpy(p->p_title.title2,get_title_from_id(id));
  p->p_title.pick = -1;
  p->score = remain_score;
}

void send_question(protocol *p, title t){
  char f_name[256] = "title/";
  strcat(f_name,get_title_fname(t));                /*f_name = "title/ title_name"*/
  strcat(p->p_question.title,get_title_fname(t));   /*p_question.title = title_name*/
  int q_num = p->p_q_num;
  p->p_q_num = q_num + 1;
  if(0 <= q_num && q_num <= 3){
    p->p_question.number_answers = 4;
  } else if(4 <= q_num && q_num <= 6){
    p->p_question.number_answers = 3;
  } else{
    p->p_question.number_answers = 2;
  }
  get_question_from_title(f_name,&(p->p_question),q_num);
}

void check_answer(protocol *p){
  int right_answer = find_right_answer(p);
  printf("VAprint: right_answer = %d\n",right_answer);
  int remain_score = get_remain_score(p->p_answer,right_answer);
  printf("VAprint: remain_score = %d\n",remain_score);
  if(remain_score > 0){
    if(p->p_q_num == 8){
      p->p_state = AUTHENTICATE;
      strcpy(p->p_message,get_right_answer_message(right_answer));
      strcat(p->p_message,"\nYou are winner");
      clear_history(p->p_user_info);
    } else{
      send_titles(p,remain_score);
      strcpy(p->p_message,get_right_answer_message(right_answer));
    }
  } else{
    p->p_state = AUTHENTICATE;
    strcpy(p->p_message,get_right_answer_message(right_answer));
    strcat(p->p_message,"\nYou are lose");
    clear_history(p->p_user_info);
  }
}
