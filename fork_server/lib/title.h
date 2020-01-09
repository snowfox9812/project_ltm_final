#ifndef TITLE_H
#define TITLE_H
typedef struct title{
  char title1[256];
  char title2[256];
  int pick; /*pick = 0 if pick title 1, pick = 1 if pick title 2*/
}title;

void set_empty_title(title *);
#endif
