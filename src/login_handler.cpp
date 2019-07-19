/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 01:18:40
*	Description：
*
==========================================================================*/

#include <dictionary.h>
LoginHandler::LoginHandler(int sockfd, char* pRequest)
{
	m_ClientFd = sockfd;
	m_pRequestInfo = pRequest;
}

void LoginHandler::handle_request()
{
	login_rs Response;
	bzero(&Response, sizeof(Response));
	Response.nType = PRO_LOGIN_RS;

	char infoBuff[_INFO_LEN] = {0};
	login_rq* pRequest = (login_rq*)m_pRequestInfo;
	if(m_pSqlHandler->id_existed(pRequest->m_pId) == _ID_NOT_EXIST)
	{
		Response.result = _ID_NOT_EXIST;
		sprintf(infoBuff, "id:%s\tpasswd:%s\t\t登录失败，ID不存在\n",
				pRequest->m_pId, pRequest->m_pPasswd);
		LOG("notice", infoBuff);
	}else{
		if(m_pSqlHandler->check_login_info(pRequest->m_pId, pRequest->m_pPasswd) == _CORRECT_INFO)
		{
			if(m_pSqlHandler->check_user_online(pRequest->m_pId) == _USER_ONLINE)
			{
				Response.result = _USER_ONLINE;
			}else{
				sprintf(infoBuff, "id:%s\tpasswd:%s\t\t登录成功\n",
						pRequest->m_pId, pRequest->m_pPasswd);
				LOG("notice", infoBuff);
				strcpy(m_pUserID, pRequest->m_pId);

				Response.result = _LOGIN_SUCCESS;
				m_pSqlHandler->set_user_state(pRequest->m_pId, 1);

				string strUserName;
				m_pSqlHandler->get_name_by_id(pRequest->m_pId, strUserName);
				strcpy(Response.m_pName, strUserName.c_str());
				strcpy(m_pUserName, strUserName.c_str());

			}	
		}else{	
			Response.result = _PASSWD_WRONG;
		}
	}
	
	if(write(m_ClientFd, &Response, sizeof(Response)) <= 0)
	{
		LOG("fatal", "fail to sent msg to client");
	}

}
