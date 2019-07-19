/*==========================================================================
*   Copyright (C) 2019 wang. All rights reserved.
*   
*	Authors:	wanglongfei(395592722@qq.com)
*	Date:		2019/07/03 04:35:26
*	Description：
*
==========================================================================*/

#pragma once
#include <dictionary.h>
class MySQL;
class MysqlHandler
{
protected:
	MySQL* sqlHandler;
public:
	MysqlHandler();
	MysqlHandler(string, string, string, string);
	int connect(string, string, string, string);
	int insert_user(string, string, string);
	int id_existed(string);
	int check_login_info(string, string);
	int check_user_online(string);
	int get_name_by_id(string, string&);
	int set_user_state(string, int);
};
