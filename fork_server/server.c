#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>

#include "lib/protocol.h"
#include "helper/login_helper.h"
#include "helper/play_helper.h"

#define PORT    5550
#define BACKLOG 10

int s_listenfd = -1;          /*server file descriptor*/
struct sockaddr_in s_server;  /*server address struct*/

fd_set s_readfds;             /*temp reading file descriptors set*/
fd_set s_sockfds;             /*reading file descriptors set*/

int init_server(void);        /*initialize server*/
void init_fds(void);          /*initialize file descriptors sets*/
int accept_new_conn();        /*accept come in connection, return -1 if fail*/

int main(int argc, char const *argv[]) {
  if(!init_server()){
    exit(-1);
  }

  init_fds();

  while(1){
    s_readfds = s_sockfds;

    if(select(FD_SETSIZE, &s_readfds, NULL, NULL, NULL) < 0){
      perror("select");
      exit(EXIT_FAILURE);
    }

    int i;
    for(i=0; i< FD_SETSIZE; i++){
      if(FD_ISSET(i, &s_readfds)){
        if(i == s_listenfd){
          if(accept_new_conn() < 0){
            exit(EXIT_FAILURE);
          }
        } else{
          protocol p;
          int bytes_recv = recv(i,&p,sizeof(protocol),0);
          if(bytes_recv < 0){
            // Recv() error
            printf("receiving error\n" );
          } else if(bytes_recv == 0){
            // Clients disconnect
          } else {
            int state = p.p_state;
            printf("%s\n", p.p_message);
            if(state == CONNECTED){
              if(!strcmp(p.p_message,WANT_TO_SIGNUP)){
                allow_signup(&p);
              } else if(!strcmp(p.p_message,WANT_TO_SIGNIN)){
                allow_signin(&p);
              }
            } else if(state == SIGNUP){
              do_signup(&p);
            } else if(state == UNAUTHENTICATE){
              do_signin(&p);
            } else if (state == AUTHENTICATE) {
              if(!strcmp(p.p_message,WANT_TO_PLAY)){
                send_titles(&p,200);  /*change state to PLAYING then send 2 titles to client*/
              } else if(!strcmp(p.p_message,"i want to signout")){
                //TODO switch state
              } else{
                //TODO
              }
            } else if(state == PLAYING){
              if(!strcmp(p.p_message,SEND_TITLE)){
                title t = p.p_title;
                send_question(&p, t);
              } else if(!strcmp(p.p_message,SEND_ANSWER)){
                check_answer(&p);
              } else{
                //TODO
              }
            } else{
              //TODO
            }

            send(i,&p,sizeof(protocol),0);
          }
        }
      }
    }
  }
  return 0;
}

int init_server(){
  if((s_listenfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket");
    return 0;
  }

  bzero(&s_server, sizeof(s_server));
  s_server.sin_family = AF_INET;
  s_server.sin_port = htons(PORT);
  s_server.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(s_listenfd,(struct sockaddr*)&s_server, sizeof(s_server)) == -1){
    perror("bind");
    return 0;
  }

  if(listen(s_listenfd,BACKLOG) == -1){
    perror("listen");
    return 0;
  }

  return 1;
}

void init_fds(){
  FD_ZERO(&s_readfds);
  FD_ZERO(&s_sockfds);

  if(s_listenfd != -1){
    FD_SET(s_listenfd, &s_sockfds);
  }
}

int accept_new_conn(){
  struct sockaddr_in client;
  int sin_size = sizeof(struct sockaddr_in);

  int temp = accept(s_listenfd, (struct sockaddr*)&client, &sin_size);

  if(temp < 0){
    perror("accept");
    return -1;
  }

  printf("Server: connect from host %s, port %hd.\n",
          inet_ntoa(client.sin_addr),
          ntohs(client.sin_port));

  FD_SET(temp, &s_sockfds);
  return temp;
}
