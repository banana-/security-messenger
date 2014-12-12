#ifndef __COMMON_H
#define __COMMON_H

#include <algorithm>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <map>

#include "const.h"


//用户请求类型
typedef enum req_type{
	LOGIN,//登录请求
	LOGOUT,//退出请求
	NORMAL
}req_type;

//用户信息
typedef struct user_mess{
	req_type type;
	char mess[MAXLINE];
}user_mess;

//用户请求信息
typedef struct user_request{
	char name[20];
	char pwd[20];
}user_req;

void str_echo(int);
void str_cli(FILE *, int);

typedef void Sigfunc(int);
Sigfunc *signal(int, Sigfunc*);

#endif
