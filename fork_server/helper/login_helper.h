#ifndef LOGIN_HELPER_H
#define LOGIN_HELPER_H

#include "../lib/protocol.h"
#include "history_helper.h"
#include "../lib/user.h"
#include "../lib/question.h"

void allow_signup(protocol *);          /*allow user to sign up*/
void do_signup(protocol *);             /*doing sign up*/
void allow_signin(protocol *);          /*allow user to sign in*/
void do_signin(protocol *);             /*doing sign in*/
void allow_signout(protocol *);
#endif
