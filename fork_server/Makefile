cc = gcc
cflag = -c -Wall

all: fork_server.o ./helper/answer_helper.o ./helper/history_helper.o ./helper/login_helper.o ./helper/play_helper.o ./helper/random_helper.o ./helper/title_helper.o ./lib/answer.o ./lib/protocol.o ./lib/question.o ./lib/title.o ./lib/user.o
	${cc} -o fork_server fork_server.o ./helper/answer_helper.o ./helper/history_helper.o ./helper/login_helper.o ./helper/play_helper.o ./helper/random_helper.o ./helper/title_helper.o ./lib/answer.o ./lib/protocol.o ./lib/title.o ./lib/user.o ./lib/question.o

fork_server.o: fork_server.c
	${cc} ${cflag} fork_server.c

./helper/answer_helper.o: ./helper/answer_helper.c
	${cc} ${cflag} ./helper/answer_helper.c

./helper/history_helper.o: ./helper/history_helper.c
	${cc} ${cflag} ./helper/history_helper.c

./helper/login_helper.o: ./helper/login_helper.c 
	${cc} ${cflag} ./helper/login_helper.c

./helper/play_helper.o: ./helper/play_helper.c
	${cc} ${cflag} ./helper/play_helper.c

./helper/random_helper.o: ./helper/random_helper.c
	${cc} ${cflag} ./helper/random_helper.c

./helper/title_helper.o: ./helper/title_helper.c
	${cc} ${cflag} ./helper/title_helper.c

./lib/answer.o: ./lib/answer.c
	${cc} ${cflag} ./lib/answer.c

./lib/protocol.o: ./lib/protocol.c
	${cc} ${cflag} ./lib/protocol.c

./lib/question.o: ./lib/question.c
	${cc} ${cflag} ./lib/question.c

./lib/title.o: ./lib/title.c
	${cc} ${cflag} ./lib/title.c

./lib/user.o: ./lib/user.c
	${cc} ${cflag} ./lib/user.c
