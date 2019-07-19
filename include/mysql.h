/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/06/07 10:40:41
*	Description：
*				 Designing a connection database interface class, 
*				 and implementing some common operations such as insert, 
*				 delete, update in the interface class.
==========================================================================*/

#pragma once
#include <dictionary.h>

class MySQL 
{ 
private: 
	MYSQL* m_pConnect; 
	MYSQL_RES* m_pResult; 
	MYSQL_ROW m_Row; 
	bool m_bConnectFlag; 

public: 
	MySQL(); 
	MySQL(MYSQL *pConnect); 
	~MySQL(); 
	bool connect(string, string, string, string);
	void disconnect(); 
	void show_databases(); 
	bool create_table(string); 
	bool use_database(string); 
	bool delete_database(string); 
	void show_tables(); 
	bool create_table(string, string); 
	vector<vector<string> > select_item(string, string, string);
	vector<vector<string> > get_fetch_result();
	bool insert_item(string, string); 
	bool insert_item(string, string, string); 
	bool delete_item(string, string); 
	bool update_item(string, string, string); 
	bool query(string); 
	bool query(string, vector<vector<string> >&);
	void show_result(); 
}; 

