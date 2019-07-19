/*==========================================================================
 *   Copyright (C) 2019 wang. All rights reserved.
 *   
 *	Authors:	wanglongfei(395592722@qq.com)
 *	Date:		2019/06/09 01:51:19
 *	Description：
 *
 ==========================================================================*/

#pragma once

#include <dictionary.h>
//#include <task_interface.h>
class Itask;
using namespace std;

/*class of thread pool manager*/
class ThreadPool
{
private:
	//int m_nThreadNum;
	int m_nMinThread;
	int m_nMaxThread;
	int m_nLiveThread;
	int m_nBusyThread;
	int m_nNeedDestoryThread;
	pthread_t *m_pThreadList;
	pthread_t m_ManagerTid;

protected:
	static vector<Itask*> m_vecTaskQueue;
	static pthread_mutex_t m_pWorkMutex;
	static pthread_cond_t m_pCondNoTask;
public:
	static bool m_bShutdown;
	static bool m_bManagerFlag;

protected:
	static void* worker_func(void *threadData);
	static void* manager_func(void *threadData);
	//static int MoveToIdle(pthread_t tid);
	//int MoveToBusy(pthread_t tid);

public:
	ThreadPool(int minNum, int maxNum);
	int add_task(Itask *task);
	int stop_all_thread();
	int get_task_num();
	int create_thread();
};

