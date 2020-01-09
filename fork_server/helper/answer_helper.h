#ifndef ANSWER_HELPER_H
#define ANSWER_HELPER_H
#include "../lib/protocol.h"
#include "../lib/answer.h"

int find_right_answer(protocol *);
int get_remain_score(answer, int);
char *get_right_answer_message(int);
#endif
