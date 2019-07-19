/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 04:21:04
*	Description：
*
==========================================================================*/

#pragma once
//#include <dictionary.h>
#include <task_interface.h>
#include <affair_access.h>

class CMyTask: public Itask
{   
public:
	AffairAccess* m_pAffairAccess;
	char m_IP[16];

public:
	CMyTask();    
	void deal_request(int clientfd);
	int run();
	~CMyTask();
};
