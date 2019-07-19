/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/06/25 05:42:38
*	Description：
*
==========================================================================*/
#pragma once

#include <cstdio>
#include <string>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <mysql/mysql.h> 
#include <time.h>
#include <typeinfo>

#include <thread_pool.h>
#include <task_deal_request.h>
#include <mylog.h>
#include <config.h>
#include <mysql.h>
#include <task_interface.h>
#include <mysql_handler.h>
#include <affair_handler.h>
#include <register_handler.h>
#include <affair_access.h>
#include <login_handler.h>

using namespace std;


//数据大小定义
#define _MEAN_LEN	256
#define _WORD_LEN	128
#define _ID_LEN		12
#define _NAME_LEN	64
#define _PASSWD_LEN	64
#define _INFO_LEN	256

//网络数据信息定义
#define	BUF_SIZE	1500
#define	LISTEN		128
#define	IP_SIZE		16
#define	IP			"127.0.0.1"
#define	SERVER_PORT	9999
#define	EPOLL		1
#define	TIME_OUT	3
#define	DEFCOUNT 	10


//定义协议类型
typedef int PackType;
#define BASE 0

#define PRO_REGISTER_RQ		BASE+1
#define PRO_REGISTER_RS 	BASE+2

#define PRO_LOGIN_RQ 		BASE+3
#define PRO_LOGIN_RS 		BASE+4

#define PRO_LOOKUP_WORD_RQ 	BASE+5
#define PRO_LOOKUP_WORD_RS 	BASE+6

#define PRO_LOOKUP_MEAN_RQ 	BASE+7
#define PRO_LOOKUP_MEAN_RS 	BASE+8


//注册及登录请求的结果
#define _CORRECT_INFO		BASE+1
#define _PASSWD_WRONG 		BASE+2
#define _ID_EXIST			BASE+3
#define _ID_NOT_EXIST		BASE+4
#define _USER_ONLINE		BASE+5
#define _USER_OFFLINE		BASE+6
#define _REGISTER_SUCCESS	BASE+7
#define _LOGIN_SUCCESS		BASE+8

#define FALSE 	0
#define TRUE	1


typedef struct node0
{
	PackType nType;
	string strMean;
}lookup_word_rq;

typedef struct node1
{
	PackType nType;
	string strWord;
}lookup_word_rs;

typedef struct node3
{
	PackType nType;
	string strWord;
}lookup_mean_rq;

typedef struct node4
{
	PackType nType;
	string strMean;
}lookup_mean_rs;

typedef struct node5
{
	PackType nType;
	char m_pId[_ID_LEN];
	char m_pName[_NAME_LEN];
	char m_pPasswd[_PASSWD_LEN];
}register_rq;

typedef struct node6
{
	PackType nType;
	int result;
}register_rs;

typedef struct node7
{
	PackType nType;
	char m_pId[_ID_LEN];
	char m_pPasswd[_PASSWD_LEN];
}login_rq;

typedef struct node8
{
	PackType nType;
	int result;
	char m_pName[_NAME_LEN];
}login_rs;

