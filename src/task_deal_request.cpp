/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/06/25 11:50:07
*	Description：
*
==========================================================================*/

//#include <dictionary.h>
#include <task_deal_request.h>


CMyTask::CMyTask()
{
	bzero(m_IP, sizeof(m_IP));
	m_pAffairAccess = new AffairAccess();
}

CMyTask::~CMyTask()
{
	if(m_pAffairAccess)
	{
		delete m_pAffairAccess;
		m_pAffairAccess = NULL;
	}
}


void CMyTask::deal_request(int clientfd)
{
	int len;
	char buf[BUF_SIZE] = {0};
	m_pAffairAccess->set_clientfd(clientfd);
	while((len = read(clientfd, buf, sizeof(buf))) > 0)
	{
		m_pAffairAccess->set_buff(buf);
		m_pAffairAccess->handle_affair();
	}

	if(len == 0)
	{
		m_pAffairAccess->set_user_offline();
	}
}

int CMyTask::run()
{
	int serverfd = *(int*)m_ptrData;
	struct sockaddr_in clientaddr;
	socklen_t size = sizeof(clientaddr);
	int clientfd = accept(serverfd, (struct sockaddr*)&clientaddr, &size);

	char infoBuff[_INFO_LEN] = {0};
	sprintf(infoBuff, "Accept a connection [client_ip: %s] [client_port: %d]\n", 
			inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, m_IP, sizeof(m_IP)),
			ntohs(clientaddr.sin_port)
		  );
	LOG("notice", infoBuff);

	this->deal_request(clientfd);
	return 0;
}
