#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <arpa/inet.h>
#include <termios.h>
#include <time.h>
#include <signal.h>
#include "lib/protocol.h"
#include "lib/login_signup.h"
#include "lib/play.h"
//-----------------------------
#define FAIL -1

//-----------------------------
protocol p;
void  INThandler(int sig)
{
     char  c;
     signal(sig, SIG_IGN);              /* disable Ctrl-C           */
     printf("Do you really want to quit? [y/n] ");
     c = getchar();                     /* read an input character  */
     if (c == 'y' || c == 'Y'){
//gui thong tin
		FILE *fptr;
		char buffer[1000];
		char temp[1000];
		fptr = fopen("client_protocol_data.txt","w");       /* if it is y or Y, then    */
          p.p_state = AUTHENTICATE;
          strcpy(p.p_message,WANT_TO_SIGNOUT);

  		sprintf(buffer, "%d", p.p_state);
		fputs(buffer,fptr);
		for (int i = 1; i <= 20 ; i++) {
			fputs("\n",fptr);
		}
		fputs(p.p_message,fptr);
		fclose(fptr);

		fptr = fopen("client_protocol_data.txt" ,"r");
		while(fgets(buffer, sizeof(buffer), fptr) != NULL) {
			strcat(temp,buffer);
		}
		fclose(fptr);
		send(sockfd,temp,sizeof(temp),0);
//gui thong tin
          exit(0);                      /* exit.  Otherwise,        */
     }
     else
          signal(SIGINT, INThandler);   /* reinstall the handler    */
}
//-----------------------------
int createConnection(int PORT,char IP[10])
{
		
	struct sockaddr_in serverAddr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		printf("Can not creat socket!\n");
		return FAIL;
	}
	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	if(connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
	printf("Connect to server fail!\n");
	return FAIL;
	}
	printf("Connected to server!\n");
	return 1;
}
void printMenu()
{
	printf("*************----------------**************\n");
	printf("*****           1.LOGIN               *****\n");
	printf("*****           2.SIGNUP              *****\n");
	printf("*****           3.QUIT                *****\n");
	printf("*************----------------**************\n");
	printf("	Please enter your choice : \n");
	printf("      **: ");
}
int main(int argc, char* argv[])
{
	signal(SIGINT,INThandler);
	int n,t,i;
	char c;
	i=createConnection(5550,argv[1]);
	if (i==FAIL)return 1;
	while(1){
		t=-1; // bien t check trang thai cua signup
		printMenu();
		scanf("%d",&n);
		while(getchar()!='\n');
		if(n==3) return 0;
		switch(n){
			case 1: 
				loop:
				//printf("%d %s %s\n",p.p_state,p.p_user_info.user_id,p.p_user_info.password );
				t=signIn(&p);
				if (t==-1) continue;


				send(sockfd,&p,sizeof(protocol),0);

				
				recv(sockfd,&p,sizeof(protocol),0);
				switch(p.p_state){
					case CONNECTED: 
						printf("\n%s\n",p.p_message);
						do{
							printf("Are you want to re-login?(y/n)");
							scanf("%c",&c);while(getchar()!='\n');
						}while(c!='y'&&c!='Y'&&c!='n'&&c!='N');
						if (c=='y'||c=='Y') goto loop;
						continue;
						break;
					case AUTHENTICATE:
						printf("\nLogin success!\n");
						i=play(&p,sockfd);
						if (i==0) continue;
						break;
				}
				break;
			case 2: 
				t=signUp(&p);
				if(t==0||t==-1) continue;


				send(sockfd,&p,sizeof(protocol),0);


				recv(sockfd,&p,sizeof(protocol),0);
				if (strcmp(p.p_message,SIGNUP_FAIL)==0){
					printf("\n%s\n","This user_id is not valid or is exist" );			
				}else printf("\n%s\n","Sign up success !!" );
				break;
		}
		
	}
}
