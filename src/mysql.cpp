/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/06/07 10:40:41
*	Description：
*==========================================================================*/

#include <mysql.h>

MySQL::MySQL() 
{ 
	m_pConnect = mysql_init(NULL); 
	if(m_pConnect == NULL)
	{
		LOG("warning", "Fatal error: fail to init mysql.");
		exit(-1);
	}
	m_pResult = NULL; 
	m_bConnectFlag = false; 
}

bool MySQL::connect(string strHost, string strUser, string strPasswd, string strDB)
{ 
	if(!m_pConnect) 
	{ 
		return false; 
	} 
	if(!mysql_real_connect(m_pConnect, strHost.data(), strUser.data(), 
						   strPasswd.data(), strDB.data(), 0, NULL, 0)) 
	{ 
		string info = "Fatal error: fail to connect databases.";
		info += "[host: " + strHost + "] [user: " + strUser + "] [passwd: " 
				+ strPasswd + "] [DB: "	+ strDB + "]";
		LOG("warning", info.c_str());
		return false; 
	}else{
		string info = "connect databases sucess!";
		info += "[host: " + strHost + "] [user: " + strUser + "] [passwd: " 
				+ strPasswd + "] [DB: "	+ strDB + "]";
		LOG("notice", info.c_str());
	}	
	return true;
}

void MySQL::disconnect() 
{ 
	if(m_pConnect) 
		mysql_close(m_pConnect); 
	m_pConnect = NULL; 
	m_pResult = NULL; 
	m_bConnectFlag = false; 
}

MySQL::MySQL(MYSQL *pConnect) 
{ 
	m_pConnect = mysql_init(pConnect); 
	m_bConnectFlag = false; 
}

MySQL::~MySQL() 
{ 
	if(m_bConnectFlag) 
		disconnect(); 
	m_pConnect = NULL; 
	mysql_free_result(m_pResult); 
	m_pResult = NULL; 
}

void MySQL::show_databases() 
{ 
	string str = "show databases"; 
	if(mysql_query(m_pConnect, str.c_str())) { 
		return; 
	} 
	show_result(); 
}

bool MySQL::create_table(const string strDBName) 
{ 
	string str = "create database if not exists " + strDBName; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		return false; 
	} 
	return true;
}

bool MySQL::use_database(const string strDBName) 
{ 
	string str = "use " + strDBName; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		return false; 
	} 
	return true;
}

bool MySQL::delete_database(const string strDBName) 
{ 
	string str = "drop database " + strDBName; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		return false; 
	} 
	return true;
}

void MySQL::show_tables() 
{ 
	string str = "show tables"; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		return; 
	} 
	show_result(); 
}


bool MySQL::create_table(const string strTable, const string strElements) 
{ 
	string str = "create table " + strTable + "(" + strElements + ")"; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		return false; 
	} 
	return true;
} 

vector<vector<string> > MySQL::select_item(const string strTable, const string strElements, const string strLimit)
{ 
	vector<vector<string> > v2_strResult; 
	string str = "select " + strElements + " from " + strTable + "where " + strLimit; 
	if(mysql_query(m_pConnect, str.c_str())) { 
		return v2_strResult; 
	} 
	return get_fetch_result();
} 

vector<vector<string> > MySQL::get_fetch_result()
{
	vector<vector<string> > v2_strResult; 
	m_pResult = mysql_use_result(m_pConnect); 
	while ( (m_Row = mysql_fetch_row(m_pResult)) != NULL ) { 
		unsigned int i = 0; 
		vector<string> v_strTmp; 
		while (i < mysql_num_fields(m_pResult)) 
			v_strTmp.push_back(m_Row[i++]); 
		v2_strResult.push_back(v_strTmp); 
	} 
	mysql_free_result(m_pResult); 
	m_pResult = NULL; 
	return v2_strResult; 
}

void MySQL::show_result() { 
	m_pResult = mysql_use_result(m_pConnect); 
	cout << "****************The result as following:******************" << endl; 
	while ( (m_Row = mysql_fetch_row(m_pResult)) != NULL ) 
	{ 
		unsigned int i = 0; 
		while (i < mysql_num_fields(m_pResult)) 
			cout << m_Row[i++] << "\t"; 
		cout << endl; 
	} 
	cout << "**********************************************************" << endl; 
	mysql_free_result(m_pResult); 
	m_pResult = NULL; 
} 

bool MySQL::insert_item(const string strTable, const string strVal) 
{ 
	string str = "insert into " + strTable + " values(" + strVal + ")"; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		string strInfo = "fail to insert, sql_msg: '";
		strInfo += str + "'";
		LOG("warning", strInfo.c_str());
		return false; 
	} 
	string strInfo = "insert success, sql_msg: '";
	strInfo += str + "'";
	LOG("notice", strInfo.c_str());
	return true;
} 

bool MySQL::insert_item(const string strTable, const string strVal, const string strCols) 
{ 
	string str = "insert into " + strTable + " (" + strCols + ") values (" + strVal + ")"; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		return false; 
	} 
	return true;
} 

bool MySQL::delete_item(const string strTable, const string strExpr) 
{ 
	string str = "delete from " + strTable + " where " + strExpr; 
	if(mysql_query(m_pConnect, str.c_str())) 
	{ 
		return false; 
	} 
	return true;
} 

bool MySQL::update_item(const string strTable, const string strExprVal, const string strExprLimit) 
{ 
	string strSQL = "update " + strTable + " set " + strExprVal + " where " + strExprLimit; 
	if(mysql_query(m_pConnect, strSQL.c_str())) { 
		return false; 
	} 
	return true;
} 

bool MySQL::query(const string strSQL) 
{ 
	if(mysql_query(m_pConnect, strSQL.c_str()))
   	{ 
		return false; 
	} 
	return true;
} 

bool MySQL::query(const string strSQL, vector<vector<string> >& v2_strResult)
{
	if(mysql_query(m_pConnect, strSQL.c_str()))
   	{ 
		return false; 
	} 
	v2_strResult = get_fetch_result();
	return true;
}
/*
int main()
{
	MySQL sqlHandler;
	sqlHandler.connect("localhost", "root", "colin123", "test");
	sqlHandler.show_databases();
	if(!sqlHandler.insert_item("info", "'wanglongf', '1609040123'"))
	{
		cout << "error to insert" << endl;
	}
	if(!sqlHandler.delete_item("info", "username='wanglongfei'"))
	{
		cout << "error to delete" << endl;
	}
	sqlHandler.update_item("info", "number='123456'", "username='wanglongf'");
	sqlHandler.create_table("student", "name char(10),num char(10) primary key, sex int");
	sqlHandler.show_tables();
	vector<vector<string> > v2_str = sqlHandler.select_item("info", "*");
	vector<vector<string> > v2_str;
	sqlHandler.query("select * from info", v2_str);
	for(int i = 0; i < v2_str.size(); i++)
	{
		for(int j = 0; j < v2_str[i].size(); j++)
		{
			cout << v2_str[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
*/
