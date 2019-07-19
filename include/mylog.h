/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/06/05 09:44:52
*	Description：
*				function: To define a class of log for other procedures to 
*						  debug and record import information
*
*			    technology: Lazy Singleton Model + Mutex 
*
==========================================================================*/

#pragma once
#include <dictionary.h>
using namespace std;

#define TIME_STD 0
#define TIME_LOG 1
#define NOTICE 	"notice"
#define WARNING "warning"
#define DEBUG 	"debug"
#define FATAL	"fatal"

#define INIT_LOG(str) Log::get_log_instance(str)
#define LOG(level, content, expr...) \
		Log::pGlobalLogInstance->log(level, content, (unsigned int)pthread_self(), __FILE__, __func__, __LINE__, ##expr)

class Log
{
private:
	string m_strPrefix;
	string m_strPath;
	string m_strCurrentTime;
	FILE* m_pNoticeFileHandler;
	FILE* m_pWarningFileHandler;
	FILE* m_pFatalFileHandler;
	FILE* m_pDebugFileHandler;

public:
	static Log* pGlobalLogInstance;
	static pthread_mutex_t m_mutexLock;

protected:
	Log(const char* pPathAndPrefix);
	Log();

public:
	string get_current_time(int nType);
	string get_format_msg(const char* pLevel, const char* pMsg, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine);
	void log(const char* pLevel, const char* pMsg, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine, int nExpr = 1);
	void build_symbolic_link(const char* pSrcFile, const char* pDestFile);
	void notice(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine);
	void warning(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine);
	void fatal(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine);
	void debug(const char* pContent, unsigned int nTid, const char* pFileName, const char* pFuncName, int nLine);

public:
	static Log* get_log_instance(const char* pPathAndPrefix);
	~Log();
};

