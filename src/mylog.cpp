/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/06/08 01:48:34
*	Description：
*
==========================================================================*/

#include "mylog.h"

Log::Log(){}

/*
Description:
		Create log files with string combined by specific name and time
		and create symbolic links for the latest log files 

Example:
		input:	  "../../test"
		variables:
				 strPrefix == "test"
				 strDestLogFile == "../../test.log"
				 strNoticeLogFile == "../../test.log.notice.2019060509"
				 ...
				 strNoticeSymbolicName == "test.log.notice"
				 ...
		finally: 
				 test.log.notice --> ../../test.log.notice.2019060509
				 ...
*/
Log::Log(const char* pPathAndPrefix)
{
	string strPathAndPrefix = string(pPathAndPrefix);
	int nIndexOfPrefix = strPathAndPrefix.find_last_of('/');
	string strPrefix = strPathAndPrefix.substr(nIndexOfPrefix + 1);

	string strCurrentTime = get_current_time(TIME_LOG);
	string strDestLogFile = strPathAndPrefix + string(".log");

	string strNoticeLogFile = strDestLogFile + string(".notice.") + strCurrentTime;
	string strWarningLogFile = strDestLogFile + string(".warning.") + strCurrentTime;
	string strDebugLogFile = strDestLogFile + string(".debug.") + strCurrentTime;
	
	m_pNoticeFileHandler = fopen(strNoticeLogFile.data(), "a");
	m_pWarningFileHandler = fopen(strWarningLogFile.data(), "a");
	m_pDebugFileHandler = fopen(strDebugLogFile.data(), "a");
	if(!m_pNoticeFileHandler || !m_pWarningFileHandler || !m_pDebugFileHandler)
	{
		cout << "Target file path of log is unavailable, please check!" << endl;
		exit(-1);
	}

	string strNoticeSymbolicName = strPrefix + string(".log.notice.") + strCurrentTime;
	string strWarningSymbolicName = strPrefix + string(".log.warning.") + strCurrentTime;
	string strDebugSymbolicName = strPrefix + string(".log.debug.") + strCurrentTime;
	
	build_symbolic_link(strNoticeSymbolicName.data(), (strPathAndPrefix + string(".log.notice")).data());
	build_symbolic_link(strWarningSymbolicName.data(), (strPathAndPrefix + string(".log.warning")).data());
	build_symbolic_link(strDebugSymbolicName.data(), (strPathAndPrefix + string(".log.debug")).data());
}

void Log::build_symbolic_link(const char* pSrcFile, const char* pDestFile)
{
	unlink(pDestFile);
	symlink(pSrcFile, pDestFile);
}

Log* Log::pGlobalLogInstance = NULL;
pthread_mutex_t Log::m_mutexLock = PTHREAD_MUTEX_INITIALIZER;

Log* Log::get_log_instance(const char* pPathAndPrefix)
{
	if(pGlobalLogInstance == NULL)
	{
		pthread_mutex_lock(&m_mutexLock);
		if(pGlobalLogInstance == NULL)
		{
			pGlobalLogInstance = new Log(pPathAndPrefix);
		}
		pthread_mutex_unlock(&m_mutexLock);
	}
	return pGlobalLogInstance;
}

Log::~Log()
{
	fclose(m_pNoticeFileHandler);
	fclose(m_pWarningFileHandler);
	fclose(m_pDebugFileHandler);
	pthread_mutex_destroy(&m_mutexLock);
	delete pGlobalLogInstance;
}

void Log::log(const char* pLevel, const char* pMsg, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine, int nExpr)
{
	if(pGlobalLogInstance == NULL)
	{
		cout << "The log has not been initialized and please initialize it first. " << endl;
		exit(-1);
	}

	if(!nExpr)
		return;

	if(strcmp(pLevel, "notice") == 0)
	{
		pGlobalLogInstance->notice(pMsg, nTid, pFileName, pFuncName, nLine);
	}else if(strcmp(pLevel, "warning") == 0){
		pGlobalLogInstance->warning(pMsg, nTid, pFileName, pFuncName, nLine);
	}else if(strcmp(pLevel, "fatal") == 0){
		pGlobalLogInstance->fatal(pMsg, nTid, pFileName, pFuncName, nLine);
	}else if(strcmp(pLevel, "debug") == 0){
		pGlobalLogInstance->debug(pMsg, nTid, pFileName, pFuncName, nLine);
	}else{
		char pTmpTip[64] = {0};
		sprintf(pTmpTip, "No such a level of log named `%s`.", pLevel);
		pGlobalLogInstance->warning(pTmpTip, nTid, pFileName, pFuncName, nLine);
	}

}

void Log::notice(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine)
{
	string strInfoMsg = get_format_msg(NOTICE, pContent, nTid, pFileName, pFileName, nLine);
	pthread_mutex_lock(&m_mutexLock);
	fputs(strInfoMsg.data(), m_pNoticeFileHandler);
	fflush(m_pNoticeFileHandler);
	pthread_mutex_unlock(&m_mutexLock);
}

void Log::warning(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine)
{
	string strInfoMsg = get_format_msg(WARNING, pContent, nTid, pFileName, pFileName, nLine);
	pthread_mutex_lock(&m_mutexLock);
	fputs(strInfoMsg.data(), m_pWarningFileHandler);
	fflush(m_pWarningFileHandler);
	pthread_mutex_unlock(&m_mutexLock);
}

void Log::fatal(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine)
{
	string strInfoMsg = get_format_msg(FATAL, pContent, nTid, pFileName, pFileName, nLine);
	pthread_mutex_lock(&m_mutexLock);
	fputs(strInfoMsg.data(), m_pWarningFileHandler);
	fflush(m_pWarningFileHandler);
	pthread_mutex_unlock(&m_mutexLock);
}

void Log::debug(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine)
{
	string strInfoMsg = get_format_msg(DEBUG, pContent, nTid, pFileName, pFileName, nLine);
	pthread_mutex_lock(&m_mutexLock);
	fputs(strInfoMsg.data(), m_pDebugFileHandler);
	fflush(m_pDebugFileHandler);
	pthread_mutex_unlock(&m_mutexLock);
}

/*
Description:
		Get format time which required by this function
Example:
		intput: TIME_STD
		return: 2019-06-05 09:44:52
*/
string Log::get_current_time(int nType)
{
	time_t timep;
	time (&timep);
	char szTemp[64];
	if(nType == TIME_LOG)
		strftime(szTemp, sizeof(szTemp), "%Y%m%d%H",localtime(&timep) );
	if(nType == TIME_STD)
		strftime(szTemp, sizeof(szTemp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
	this->m_strCurrentTime = szTemp;

	return this->m_strCurrentTime;
}

string Log::get_format_msg(const char* pLevel, const char* pMsg, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine)
{
	char szBufLine[8] = {0};
	char szBufTid[16] = {0};
	sprintf(szBufLine, "%d", nLine);
	sprintf(szBufTid, "0x%x", nTid);
	string strMsg = "";
	strMsg += string("{") + string(pLevel) + string("} [tid:") + string(szBufTid) + string("] [file:") + string(pFileName);
	strMsg += string("] [func:") + string(pFuncName) + string("] [line:") + string(szBufLine) + string("]\nMSG:") + string(pMsg);
	cout << strMsg << "\n\n";
	return get_current_time(TIME_STD) + strMsg + string("\n\n");		
}

/*
int main()
{
	//Log log("../test");
	//cout << log.get_format_msg("warning", "this is a test", 82171298, __FILE__, __func__, __LINE__) << endl;
	//Log::get_log_instance("../aa")->notice("this is a test", 82171298, __FILE__, __func__, __LINE__);
	//Log::get_log_instance("../aa")->warning("this is a test", 82171298, __FILE__, __func__, __LINE__);
	//Log::get_log_instance("../aa")->debug("this is a test", 82171298, __FILE__, __func__, __LINE__);
	INIT_LOG(./test);
	LOG(warning, this is just a test, 1 == 2);
	LOG(notice, this is just a test);

	return 0;
}
*/


