/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 09:18:55
*	Description：
*
==========================================================================*/

#pragma once

#include <dictionary.h>
//#include <affair_handler.h>
class LoginHandler: public AffairHandler
{
public:
	LoginHandler(int sockfd, char* pRequest);
	virtual void handle_request();
};
