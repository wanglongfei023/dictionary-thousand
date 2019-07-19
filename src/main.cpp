/*==========================================================================
 *   Copyright (C) 2019 wang. All rights reserved.
 *   
 *	Authors:	wanglongfei(395592722@qq.com)
 *	Date:		2019/06/09 01:54:08
 *	Description：
 *
 ==========================================================================*/

#include <dictionary.h>

int thread_socketCreate(void)
{
	struct sockaddr_in serveraddr;
	int socketfd;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(socketfd,(struct sockaddr*)&serveraddr, sizeof(serveraddr));
	listen(socketfd, 128);
	return socketfd;
}

int main() {
	Config cfg("../conf/config.ini");
	INIT_LOG(cfg.get("log_path").c_str());
	struct epoll_event eparr[1];
	struct epoll_event eptmp;
	int sockfd = thread_socketCreate();	
	//主控线程使用epoll监听socket事件
	int epfd = epoll_create(1);
	eptmp.data.fd = sockfd;
	eptmp.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &eptmp);
	LOG("notice", "Thread Pool Server Runing....");
	ThreadPool threadpool(5, 10);  //线程池大小为5
	while(!threadpool.m_bShutdown)
	{
		while(true)
		{
			epoll_wait(epfd, eparr, 1, -1);
			CMyTask* pTaskInstance = new CMyTask();
			pTaskInstance->set_data((void*)&sockfd);
			threadpool.add_task(pTaskInstance);
		}
	}
	close(sockfd);
	return 0;
}
