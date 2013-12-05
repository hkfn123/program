#include <stdio.h>
#include <mysql.h>
#include <errno.h>
#include "storeToMysql.h"

#define HOST "localhost"
#define UNAME "root"
#define PASSWORD ""
#define DATABASE_NAME "test"
#define PORT 0

int storeToMysql(Category category)
{
	MYSQL* mysql_conn;
	char sqlcmd[512];
	int ret = 0;
	mysql_conn = mysql_init(NULL);
	
	if(mysql_real_connect(mysql_conn,HOST,UNAME,PASSWORD,DATABASE_NAME,PORT,NULL,0))
	{
		perror("database connect unsuccessful.\n");
		return -1;
	}
	sprintf(sqlcmd,"insert into category values(%s,%s,%s,%s)",category.thumbnail,category.id,category.cat,category.type);
	printf("%s\n",sqlcmd);
	ret = mysql_query(mysql_conn,sqlcmd);
	if(ret)
	{
		perror("insert into category error.\n");
		mysql_close(mysql_conn);
		return -1;
	}
	mysql_close(mysql_conn);

	return 0;
}

