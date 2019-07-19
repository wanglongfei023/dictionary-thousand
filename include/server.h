#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <mysql/mysql.h>

#define	SERVERIP "192.168.26.130" 
#define	SERVERPORT	8000
#define	BUFSIZE	8192

//临界值定义
#define	IPSIZE		16
#define	LISTEN		128
#define	EPOLLSIZE	6000
#define MAXMEAN		32 		//每个单词最多意思个数
#define MAXLEN		512		//每条结果的最大长度

//定义协议类型
typedef int PackType;
//注册登录包标识
#define PRO_REGISTER_RQ 1
#define PRO_REGISTER_RS 2
#define PRO_LOGIN_RQ 	3
#define PRO_LOGIN_RS 	4

#define PRO_LOOKUP_RQ 	5
#define PRO_LOOKUP_RS 	6


//注册及登录请求的结果
#define _id_exist			0
#define _id_not_exist		1
#define _passwd_wrong		2
#define _register_success	3
#define _login_success		4

typedef struct node0
{
	PackType nType;
	int nRsType; //0为单词查意思 1为意思查单词
	char Content[32];
}lookup_rq;

typedef struct node1
{
	PackType nType;
	int nRsType; //0为单词查意思 1为意思查单词
	char mean[256];
	char word[MAXMEAN][MAXLEN];
}lookup_rs;

typedef struct node2
{
	PackType nType;
	char id[12];
	char name[16];
	char passwd[16];
}register_rq;

typedef struct node3
{
	PackType nType;
	int result;
}register_rs;

typedef struct node4
{
	PackType nType;
	char id[12];
	char passwd[16];
}login_rq;

typedef struct node5
{
	PackType nType;
	int result;
	char name[16];
}login_rs;

int get_mean_by_word(const char* word,char* mean);
int get_word_by_mean(const char* mean,char word[32][512]);
int client_connection();
void deal_data(char* pBuff,int sockfd);
int is_letter(char c);
int is_delimeter(char* pStart,int len);
MYSQL* connect_database();
void deal_lookup_rq(char* pBuff,int sockfd);
void deal_register_rq(char* pBuff,int sockfd);
void deal_login_rq(char* pBuff,int sockfd);
int insert_user(MYSQL* conn,char* id,char* name,char* passwd);
int is_correct(MYSQL* conn,char* name,char* passwd); //1表示正确 -1 表示用户不存在 0表示密码错
int is_id_not_existed(MYSQL* conn,char* id);






