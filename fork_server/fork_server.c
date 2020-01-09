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
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#include "lib/protocol.h"
#include "helper/login_helper.h"
#include "helper/play_helper.h"
#include "helper/history_helper.h"

#define PORT    5550
#define BACKLOG 10

int s_listenfd = -1;          /*server file descriptor*/
struct sockaddr_in s_server;  /*server address struct*/

int init_server(void);        /*initialize server*/
int accept_new_conn();        /*accept come in connection, return -1 if fail*/

void sig_chld(int signo){
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0)
		printf("[ForkingServer] Child %d terminated\n",pid);
}

int main(int argc, char const *argv[]) {
  if(!init_server()){
    exit(-1);
  }

  while(1){
	  int conn_fd = accept_new_conn();
    if(conn_fd < 0){
      continue;
    }

    pid_t pid;
		if((pid=fork())==0){
			close(s_listenfd);
      fd_set readfds;
      FD_ZERO(&readfds);
			int set_to_flag = 0, to_flag = 0;
			int rc;

      while(1){
				FD_SET(conn_fd,&readfds);	/*call it per each loop round*/
				if(set_to_flag){
					printf("start countdown\n");
					struct timeval t_out;
					t_out.tv_sec = 20;
					t_out.tv_usec = 0;
					rc = select(conn_fd+1,&readfds,NULL,NULL,&t_out);
					set_to_flag = 0;
				} else{
					rc = select(conn_fd+1,&readfds,NULL,NULL,NULL);
				}
        if(rc < 0){
          //select error
					perror("select");
					break;
        } else if(rc == 0){
          //timeout
					to_flag = 1;
					printf("Time out\n");
        } else{
          if(FD_ISSET(conn_fd,&readfds)){
            protocol p;
//Nhan thong tin           
            FILE *fptr;
            char buffer[1000];
            char temp[1000];
            fptr = fopen("server_protocol_data.txt", "w");

            int bytes_recv = recv(conn_fd,buffer,sizeof(buffer),0);            
            fputs(buffer,fptr);
            fclose(fptr);

            fptr = fopen("server_protocol_data.txt", "r");
            fgets(temp, sizeof(temp), fptr);
            p.p_state = atoi(temp);
            for (int count = 1; count <= 19; count ++ ) {
              fgets(temp, sizeof(temp), fptr);
            }
            fgets(temp, sizeof(temp), fptr);
            strcpy(p.p_message, temp);
            fclose(fptr);
//Nhan thong tin
            if(bytes_recv < 0){
              // Recv() error
              printf("receiving error\n" );
							FD_CLR(conn_fd,&readfds);
							break;
            } else if(bytes_recv == 0){
              // Clients disconnect
							printf("client %d is disconnect\n",conn_fd);
							FD_CLR(conn_fd,&readfds);
              break;
            } else{
              int state = p.p_state;
              printf("state code: %d - message: %s\n",p.p_state,p.p_message);
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
                } else if(!strcmp(p.p_message,WANT_TO_SIGNOUT)){
									clear_history(p.p_user_info);
									allow_signout(&p);
                } else{
                  //TODO
                }
              } else if(state == PLAYING){
                if(!strcmp(p.p_message,SEND_TITLE)){
                  title t = p.p_title;
                  send_question(&p, t);
									set_to_flag = 1;
                } else if(!strcmp(p.p_message,SEND_ANSWER)){
									if(to_flag){
										p.p_state = AUTHENTICATE;
										p.score = 0;
										strcpy(p.p_message,TIMEOUT);
                    clear_history(p.p_user_info);
                    to_flag = 0;
									} else{
										check_answer(&p);
									}
                }
              }


              send(conn_fd,&p,sizeof(protocol),0);
            }
          }
        }
      }
			exit(0);
		}

		signal(SIGCHLD,sig_chld);

		close(conn_fd);
	}
	close(s_listenfd);

  return 0;
}

/*--------------------------------support function--------------------------------------------*/
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

  return temp;
}
