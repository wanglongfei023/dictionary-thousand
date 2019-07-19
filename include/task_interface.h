/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 04:26:36
*	Description：
*
==========================================================================*/

#pragma once
#include <dictionary.h>
using namespace std;

class Itask 
{
protected:
	string m_strTaskName;
	void* m_ptrData;

public:
	Itask(){};
	Itask(string &taskName): m_strTaskName(taskName), m_ptrData(NULL) {}
	virtual ~Itask() {}
	virtual int run() = 0;
	void set_data(void* data);
};
