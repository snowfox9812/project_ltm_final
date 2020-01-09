#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include "protocol.h"
#include "fire_work.h"

void printMenuPlay()//pint menu
{
	printf("*************----------------**************\n");
	printf("*****           1.PLAY                *****\n");
	printf("*****           2.QUIT                *****\n");
	printf("*************----------------**************\n");
	printf("	Please enter your choice : \n");
	printf("      **: ");
}
//--------------cac ham delay-----------------
char get_answer(int time_out)//delay
{
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    char answer;

    if( poll(&mypoll, 1, time_out) )
    {
        scanf("%c", &answer);
        while(getchar()!='\n');
        return answer;
    }
    else return 'F';
}
void delay()
{
	printf(" The game will start in: \n");
	get_answer(2000);
	printf("3..");
	get_answer(2000);
	printf("2...");
	get_answer(2000);
	printf("1....\n");
	get_answer(1000);
}
//-------------------------------------
int chooseTitle(protocol *p){
	int i;
	do{
		i=-1;
		printf("------Please pick one of two titles below------\n");
		printf("Title 1: %s\n", (p->p_title).title1);
		printf("Title 2: %s\n", (p->p_title).title2);
		printf("------You pick(1 or 2): ");
		scanf("%d",&i);while(getchar()!='\n');
		i--;
	}while(i!=0&&i!=1);
	return i;
}

int printQuestion(protocol *p,int x){
	char s1[10],s2[10],s3[10],s4[10];
	int a1,a2,a3,a4;
	int i=p->p_question.number_answers;
	printf("Question %d: %s\n",x,p->p_question.question);
	printf("1. %s\n", p->p_question.answer1);
	printf("2. %s\n", p->p_question.answer2);
	if(x<8) printf("3. %s\n", p->p_question.answer3);
	if(x<5) printf("4. %s\n",p->p_question.answer4);
	printf("You have %d millions VND\n",p->score);
	printf("You have to bet within 60 seconds\n");
	answer:
	printf("You have been to bet in %d answer(s)\n", i-1);
	printf("Total amount must be equal to the amount bet are\n");
	printf("Input your bet, devided by | character, example: 100|100|0|0|\n");
	switch(i){
		case 4:
			scanf("%d|%d|%d|%d\n",&(p->p_answer).bet1,&(p->p_answer).bet2,&(p->p_answer).bet3,&(p->p_answer).bet4);
			while(getchar()!='\n');
			a1=(p->p_answer).bet1;
			a2=(p->p_answer).bet2;
			a3=(p->p_answer).bet3;
			a4=(p->p_answer).bet4;
			if(((a1+a2+a3+a4)!=p->score) || (a1!=0&&a2!=0&&a3!=0&&a4!=0)){
				printf("%d %d %d %d\n",a1,a2,a3,a4 );
				printf("Your bet is not valid, bet again\n");
				goto answer;
			}
			break;
		case 3:
			scanf("%d|%d|%d\n",&(p->p_answer).bet1,&(p->p_answer).bet2,&(p->p_answer).bet3);
			while(getchar()!='\n');
			a1=(p->p_answer).bet1;
			a2=(p->p_answer).bet2;
			a3=(p->p_answer).bet3;
			if(((a1+a2+a3)!=p->score) || (a1!=0&&a2!=0&&a3!=0)){
				printf("Your bet is not valid, bet again\n");
				goto answer;
			}
			break;
		case 2:
			scanf("%d|%d\n",&(p->p_answer).bet1,&(p->p_answer).bet2);
			while(getchar()!='\n');
			a1=(p->p_answer).bet1;
			a2=(p->p_answer).bet2;
			if(((a1+a2)!=p->score) || (a1!=0&&a2!=0)){
				printf("Your bet is not valid, bet again\n");
			}
			break;			
	}
	strcpy(p->p_message,SEND_ANSWER);
}

int play(protocol *p,int sockfd){
	int n,i;//n choose menu
	char c;
	loop:
	printMenuPlay();
	scanf("%d",&n);while(getchar()!='\n');
	if(n==2) {
		p->p_state=AUTHENTICATE;
		strcpy(p->p_message,WANT_TO_SIGNOUT);
		send(sockfd,p,sizeof(protocol),0);
		recv(sockfd,p,sizeof(protocol),0);
		return 0;
	}
	else if (n==1){
		strcpy(p->p_message,WANT_TO_PLAY);
		p->p_q_num=0;
		send(sockfd,p,sizeof(protocol),0);
		recv(sockfd,p,sizeof(protocol),0);
		if(p->p_state != PLAYING) return -1;//server error
		do{
			printf("ARE YOU READY?(y/n): ");
			scanf("%c",&c);
		}while(c!='y'&&c!='Y'&&c!='n'&&c!='N');
		if (c=='y'||c=='Y'){
			delay();
			loopTitle:
			(p->p_title).pick = chooseTitle(p);
			strcpy(p->p_message,SEND_TITLE);
			send(sockfd,p,sizeof(protocol),0);
			recv(sockfd,p,sizeof(protocol),0);
			printQuestion(p,p->p_q_num);
			send(sockfd,p,sizeof(protocol),0);
			recv(sockfd,p,sizeof(protocol),0);

			if(p->score==-1){
				printf("Time out!\n");
				get_answer(2000);
				goto loop;
			}
			if(p->p_state==AUTHENTICATE){
				printf("%s\n",p->p_message );
				if(strstr(p->p_message,"You are winner")!=NULL){
					printf("Your price: %d million(s)\n", p->score);
					fire_work();
					fire();
					get_answer(2000);
					fire_work1();
					get_answer(2000);
					fire_work();
					get_answer(2000);
					fire_work1();
					get_answer(2000);
					fire_work();
				}
				goto loop;

				
			}else {
				printf("%s\n",p->p_message );
				printf("So diem hien tai: %d\n",p->score);
				goto loopTitle;
				
			}
			
			return 1;
		}
		else goto loop;
	}
	else goto loop;
}