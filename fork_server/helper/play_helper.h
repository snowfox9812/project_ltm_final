#ifndef PLAY_HELPER_H
#define PLAY_HELPER_H

#include "../lib/protocol.h"
#include "../lib/title.h"
#include "../lib/user.h"

void send_titles(protocol *, int);      /*send 2 titles to user*/
void pick_2_titles(char *, char *);     /*pick 2 titles from title directory*/
void send_question(protocol *, title ); /*send question to client base on his/her title*/
void check_answer(protocol *);
#endif
