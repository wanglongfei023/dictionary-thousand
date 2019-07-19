/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 01:26:16
*	Description：
*
==========================================================================*/

#include <dictionary.h>
using namespace std;

MysqlHandler::MysqlHandler()
{
	sqlHandler = new MySQL();
}

MysqlHandler::MysqlHandler(string strHost, string strUser, string strPasswd, string strDB)
{
	sqlHandler = new MySQL();
	sqlHandler->connect(strHost, strUser, strPasswd, strDB);
}

int MysqlHandler::connect(string strHost, string strUser, string strPasswd, string strDB)
{
	return sqlHandler->connect(strHost, strUser, strPasswd, strDB) ? TRUE :FALSE;
}

int MysqlHandler::insert_user(string strID, string strName, string strPasswd)
{
	string strVal = "'" + strID + "', '" + strName + "', '" + strPasswd + "'";
	string strCols = "id, name, passwd";
	return sqlHandler->insert_item("user", strVal, strCols) ? TRUE : FALSE;
}

int MysqlHandler::id_existed(string strID)
{
	string strSql = "select * from user where id='";
	strSql += strID + "'";
	vector<vector<string> > v2_result;
	sqlHandler->query(strSql, v2_result);
	return v2_result.size() == 0 ? _ID_NOT_EXIST : _ID_EXIST;
}

int MysqlHandler::check_user_online(string strID)
{
	string strSql = "select state from user where id='";
	strSql += strID + "'";
	vector<vector<string> > v2_result;
	sqlHandler->query(strSql, v2_result);

	return v2_result[0][0] == "0" ? _USER_OFFLINE : _USER_ONLINE;
}
int MysqlHandler::check_login_info(string strID, string strPasswd)
{
	string strSql = "select passwd from user where id='";
	strSql += strID + "'";
	vector<vector<string> > v2_result;
	sqlHandler->query(strSql, v2_result);
	
	return v2_result[0][0] == strPasswd ? _CORRECT_INFO : _PASSWD_WRONG;
}

int MysqlHandler::get_name_by_id(string strID, string& strName)
{
	string strSql = "select name from user where id='";
	strSql += strID + "'";
	vector<vector<string> > v2_result;
	sqlHandler->query(strSql, v2_result);
	
	if(v2_result[0][0] == "")
	{
		return FALSE;
	}else{
		strName = v2_result[0][0];
		return TRUE;
	}
}

int MysqlHandler::set_user_state(string strID, int nState)
{
	char sqlBuff[_INFO_LEN];
	sprintf(sqlBuff, "update user set state=%d where id='%s'", nState, strID.c_str());
	return sqlHandler->query(sqlBuff) ? TRUE : FALSE;
}
