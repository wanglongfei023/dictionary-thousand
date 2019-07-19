/*==========================================================================
 *   Copyright (C) 2019 wang. All rights reserved.
 *   
 *	Authors:	wanglongfei(395592722@qq.com)
 *	Date:		2019/06/09 01:52:35
 *	Description：
 *
 ==========================================================================*/
#include <dictionary.h>

vector<Itask*> ThreadPool::m_vecTaskQueue;
bool ThreadPool::m_bShutdown = false;
pthread_mutex_t ThreadPool::m_pWorkMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::m_pCondNoTask = PTHREAD_COND_INITIALIZER;

ThreadPool::ThreadPool(int minNum, int maxNum)
{
	this->m_nMinThread = minNum;
	this->m_nMaxThread = maxNum;
	this->m_pThreadList = new pthread_t[m_nMaxThread];
	this->create_thread();
}

void* ThreadPool::manager_func(void* threadData) 
{
	while(!m_bShutdown)
	{
		break;
	}
	return NULL;
}
void* ThreadPool::worker_func(void* threadData) 
{
	while(1)
	{
		pthread_mutex_lock(&m_pWorkMutex);

		/*
		 *Description:
		 * 		threads will suspend themseves here while task list is and 
		 *	    null `shutdown` is false
		 *NB:
		 *		`惊群效应` --> when sigal 'm_pCondNoTask'
		 */
		while(m_vecTaskQueue.size() == 0 && !m_bShutdown)
			pthread_cond_wait(&m_pCondNoTask, &m_pWorkMutex);

		if(m_bShutdown)
		{
			pthread_mutex_unlock(&m_pWorkMutex);
			pthread_exit(NULL);
		}

		vector<Itask*>::iterator iter = m_vecTaskQueue.begin();

		//get a task from task list and deal it
		Itask* pTask = *iter;
		if(iter != m_vecTaskQueue.end())
		{
			pTask = *iter;
			m_vecTaskQueue.erase(iter);
		}

		pthread_mutex_unlock(&m_pWorkMutex);
		pTask->run();

		/*
		if(pTask)
		{
			delete pTask;
			pTask = NULL;
		}
		*/
	}
	return (void*)0;
}

int ThreadPool::add_task(Itask *task) { 
	pthread_mutex_lock(&m_pWorkMutex);    
	m_vecTaskQueue.push_back(task);  
	pthread_mutex_unlock(&m_pWorkMutex);  
	pthread_cond_signal(&m_pCondNoTask);    

	return 0;
}

int ThreadPool::create_thread() { 
	for(int i = 0; i < m_nMinThread; i++)
		pthread_create(&m_pThreadList[i], NULL, worker_func, NULL);
	m_nLiveThread = m_nMinThread;
	return 0;
}

int ThreadPool::stop_all_thread() {    
	//to avoid call again
	if(m_bShutdown)
		return -1;
	printf("Now I will end all threads!\n\n");

	//wake up pending threads
	m_bShutdown = true;
	pthread_cond_broadcast(&m_pCondNoTask);

	//recycling threads' resources 
	for(int i = 0; i < m_nLiveThread; i++)
		pthread_join(m_pThreadList[i], NULL);

	delete[] m_pThreadList;
	m_pThreadList = NULL;

	pthread_mutex_destroy(&m_pWorkMutex);
	pthread_cond_destroy(&m_pCondNoTask);

	return 0;
}

int ThreadPool::get_task_num() {    
	return m_vecTaskQueue.size();
}

