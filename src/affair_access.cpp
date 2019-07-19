/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/04 10:23:09
*	Description：
*
==========================================================================*/

#include <dictionary.h>

AffairAccess::AffairAccess():m_clientfd(-1), m_pBuff(NULL), m_pHandler(NULL) {}

AffairAccess::AffairAccess(int fd)
{
	m_clientfd = fd;
	m_pBuff = NULL;
	m_pHandler = NULL;
}

AffairAccess::AffairAccess(int fd, char* pBuff = NULL)
{
	m_clientfd = fd;
	m_pBuff = pBuff;
	m_pHandler = NULL;
}

AffairAccess::~AffairAccess()
{
	if(m_pHandler)
	{
		delete m_pHandler;
		m_pHandler = NULL;
	}
}

void AffairAccess::set_buff(char* pBuff)
{
	m_pBuff = pBuff;
}

void AffairAccess::set_clientfd(int fd)
{
	m_clientfd = fd;	
}

void AffairAccess::set_user_offline()
{
	m_pHandler->set_user_offline();
}

void AffairAccess::handle_affair()
{
	PackType* pType = (PackType*)m_pBuff;
	switch(*pType)
	{
		case PRO_REGISTER_RQ:
			{
				m_pHandler = new RegisterHandler(m_clientfd, m_pBuff);
				break;
			}
		case PRO_LOGIN_RQ:
			{
				m_pHandler = new LoginHandler(m_clientfd, m_pBuff);
				break;
			}
	}
	m_pHandler->handle_request();
}
