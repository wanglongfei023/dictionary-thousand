/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 01:18:40
*	Description：
*
==========================================================================*/

#include <dictionary.h>
RegisterHandler::RegisterHandler(int sockfd, char* pRequest)
{
	m_ClientFd = sockfd;
	m_pRequestInfo = pRequest;
}

void RegisterHandler::handle_request()
{
	register_rs Response;
	Response.nType = PRO_REGISTER_RS;

	register_rq* pRequest = (register_rq*)m_pRequestInfo;
	if(m_pSqlHandler->id_existed(pRequest->m_pId) == _ID_NOT_EXIST)
	{
		m_pSqlHandler->insert_user(pRequest->m_pId, pRequest->m_pName, pRequest->m_pPasswd);
		Response.result = _REGISTER_SUCCESS;
		char infoBuff[_INFO_LEN] = {0};
		sprintf(infoBuff, "id:%s\tname:%s\tpasswd:%s\t\t注册成功\n",
				pRequest->m_pId, pRequest->m_pName, pRequest->m_pPasswd);
		LOG("notice", infoBuff);
	}else{
		Response.result = _ID_EXIST;
		char infoBuff[_INFO_LEN] = {0};
		sprintf(infoBuff, "id:%s\tname:%s\tpasswd:%s\t\t注册失败\n",
				pRequest->m_pId, pRequest->m_pName, pRequest->m_pPasswd);
		LOG("warning", infoBuff);
	}
	
	if(write(m_ClientFd, &Response, sizeof(Response)) <= 0)
	{
		LOG("fatal", "fail to sent msg to client");
	}

}
