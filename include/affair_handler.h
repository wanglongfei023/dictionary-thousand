/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 09:17:33
*	Description：
*
==========================================================================*/

#pragma once
#include <mysql_handler.h>
class AffairHandler
{
public:
	int m_ClientFd;
	char* m_pRequestInfo;
	char* m_pUserID;
	char* m_pUserName;
	MysqlHandler* m_pSqlHandler;

public:
	AffairHandler();
	AffairHandler(int sockfd, char* pRequest);
	void set_user_offline();
	virtual ~AffairHandler();
	virtual void handle_request() = 0;
};
