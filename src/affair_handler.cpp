/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 01:10:20
*	Description：
*
==========================================================================*/

#include <dictionary.h>


AffairHandler::AffairHandler()
{
	m_ClientFd = 0;
	m_pRequestInfo = NULL;
	m_pUserName = new char(_NAME_LEN);
	m_pUserID = new char(_ID_LEN);
	bzero(m_pUserID, _ID_LEN);
	bzero(m_pUserName, _NAME_LEN);
	m_pSqlHandler = new MysqlHandler("localhost", "root", "colin123", "dictionary");
}

AffairHandler::AffairHandler(int sockfd, char* pRequest)
{
	m_ClientFd = sockfd;
	m_pRequestInfo = pRequest;
	m_pSqlHandler = new MysqlHandler("localhost", "root", "colin123", "dictionary");
}

AffairHandler::~AffairHandler()
{
	/*
	if(m_pUserID)
	{
		free(m_pUserID);
		m_pUserID = NULL;
	}

	if(m_pUserName)
	{
		free(m_pUserName);
		m_pUserName = NULL;
	}
	*/
	close(m_ClientFd);
}

void AffairHandler::set_user_offline()
{
	if(m_pUserID)
	{
		m_pSqlHandler->set_user_state(m_pUserID, 0);
	}

	if(m_pUserName)
	{
		char infoBuff[_INFO_LEN] = {0};
		sprintf(infoBuff, "user:[%s] has been offline.", m_pUserName);
  		LOG("notice", infoBuff);
	}
}
