#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../lib/protocol.h"
#include "history_helper.h"

int match_history(user u, int id){
  char f_name[64] = "history/";
  strcat(f_name,u.user_id);       // history/va_dl
  if(access(f_name, F_OK) != -1){ // User has played
    FILE *f;
    f = fopen(f_name,"r");
    if(f){
      char c = fgetc(f);
      if(c == EOF){             // history file is empty
        return 0;
      } else{
        fseek(f,0,SEEK_SET);    // back to begin of file
        char temp[3] = "";
        while(c != EOF){
            fscanf(f,"%s",temp);
            if(id == atoi(temp)){
              return 1;
            }  // if match history
            c = fgetc(f);         // read line character or EOF
        }
        fclose(f);
        return 0;                 // not match
      }
    } else{
      fclose(f);
      return 0;
    }
  } else{                         // User has never played
    return 0;
  }
}

void write_history(user u, int t_id){
  FILE *f;
  char f_name[64] = "history/";
  strcat(f_name,u.user_id);
  printf("%s\n",f_name );
  f = fopen(f_name,"a");
  fprintf(f, "%d\n", t_id);
  fclose(f);
}

int match_connected_state(user u){
  char f_name[64] = "userstate/";
  strcat(f_name,u.user_id);
  FILE *f;
  f = fopen(f_name,"r");
  if(f){
    int state;
    fscanf(f,"%d",&state);
    if(state == CONNECTED)
      return 1;
    else
      return 0;
  }
  fclose(f);
  return 0;
}

void write_user_state(user u, int state){
  char f_name[64] = "userstate/";
  strcat(f_name,u.user_id);
  FILE *f;
  f = fopen(f_name,"w");
  if(f){
    fprintf(f, "%d", state);
  }
  fclose(f);
}

void clear_history(user u){
  FILE *f;
  char f_name[64] = "history/";
  strcat(f_name,u.user_id);
  printf("%s\n",f_name );
  f = fopen(f_name,"w");
  fclose(f);
}
