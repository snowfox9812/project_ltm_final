#ifndef USER_H
#define USER_H
typedef struct user{
  char user_id[32];
  char password[32];
}user;

void set_empty_user(user *);
#endif
