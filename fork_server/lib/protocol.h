#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "user.h"
#include "question.h"
#include "answer.h"
#include "title.h"

enum{
  CONNECTED, SIGNUP, UNAUTHENTICATE, AUTHENTICATE, READY, PLAYING,TIMEOUT
};

#define WANT_TO_SIGNUP  "i want to signup"
#define ALLOW_TO_SIGNUP "you are allowed to signup"
#define SIGNUP_FAIL "fail signup"
#define SIGNUP_SUCCESS "signup success"
#define WANT_TO_SIGNIN  "i want to signin"
#define ALLOW_TO_SIGNIN "you are allowed to signin"
#define WRONG_USER_ID "user id is not exist"
#define WRONG_PASSWORD "password is not correct"
#define CONNECTED_BY_OTHER "you connected in other device"
#define WANT_TO_SIGNOUT "i want to signout"
#define WANT_TO_PLAY "i want to play"
#define ALLOW_PLAY "you are allowed to play"
#define SEND_TITLE "send title"
#define SEND_ANSWER "send answer"
#define TIMEOUT "you answered too late"

typedef struct protocol{
  int p_state;
  user p_user_info;
  title p_title;
  int p_q_num;
  question p_question;
  int score;
  answer p_answer;
  char p_message[256];
}protocol;

void set_signup_state(protocol *);
#endif
