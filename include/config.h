/*==========================================================================
 *   Copyright (C) 2019 wang. All rights reserved.
 *   
 *	Authors:	wanglongfei(395592722@qq.com)
 *	Date:		2019/06/07 08:43:40
 *	Description：
 *
 ==========================================================================*/

#pragma once
#include <dictionary.h>

struct NodeInfo
{
	string strKey;
	string strValue;
	NodeInfo *pNext;
};

class Config
{
private:
	string m_strFileName;
	NodeInfo* m_pHead;
	int m_nLineNum;

protected:
	void create_head();
	void free_join();	
	void synchronize_to_file();
	void join_head(string strKey, string strValue);
	void trim(string &str);

public:
	Config(string m_strFileName);
	~Config();
	int get_line_num();
	void set(string strKey, string strValue);
	string get(string strKey, string strDefaultVal="");
	void load_conf();
	void show_all_item();
};

