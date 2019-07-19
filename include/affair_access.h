/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/04 10:41:44
*	Description：
*
==========================================================================*/

#pragma once
//#include <dictionary.h>
#include <affair_handler.h>
class AffairAccess
{
protected:
	int m_clientfd;
	char* m_pBuff;
	AffairHandler* m_pHandler;

public:
	AffairAccess();
	AffairAccess(int fd, char* pBuff);
	AffairAccess(int fd);
	~AffairAccess();
	void set_buff(char* pBuff);
	void set_clientfd(int fd);
	void set_user_offline();
	void handle_affair();
};

