#include <stdio.h>
#include <mysql.h>
#include <errno.h>
#include "storeToMysql.h"
#include <string.h>
#include "getJsonString.h"
#include "parseJson.h"

MYSQL *conn_ptr;	
char sqlcmd[8196];

char* handleStringWithString(const char* idStr);
#define BASE_URL "http://119.147.105.61:8080/app/v2/list.jsp?mode=new&"

void requestJsonStringWithId(const char* idStr)
{
	char blessUrl[255];
	char tempStr[30];	
	int i = 0;
	char* anTempStr = handleStringWithString(idStr);
//	printf("%s\n",anTempStr);
	for(i = 0;i<100;i++)
	{
			
		memset(tempStr,0,30);
		memset(blessUrl,0,255);
		strcat(blessUrl,BASE_URL);
		sprintf(tempStr,"page=%d&catid=%s",i,anTempStr);
		strcat(blessUrl,tempStr);
		//fprintf(stderr,"%s\n",blessUrl);
		char* retString = getURLContentfromURLString(blessUrl);
		fprintf(stderr,"%ld",strlen(retString));
		if(strlen(retString)>100)
		{
			parseBlessFromJsonString(retString);
		}
		else
		{
			fprintf(stderr,"break\n");
			break;
		}
		
		if(retString) free(retString);
		retString = NULL;
	}
	if(anTempStr) free(anTempStr);
}

char* handleStringWithString(const char* idStr)
{
	int i = 0;
	char* retString = NULL;
	retString = (char*)malloc(10);
	strcpy(retString,idStr);
	for(i=0;i<strlen(retString);i++)
	{	
		if(i==strlen(retString)-1) 
		{
			retString[i] = '\0';
		}
	}
	return retString;
}

int connectToMysqlServer()
{
	char* host = "10.10.4.92";
	char* user = "steven";
	char* password = "Foxconn123";
	char* db = "messageCollection";
	unsigned int port = 0;
	char* unix_socket = NULL;
	unsigned long client_flag = 0;
	
	conn_ptr = mysql_init(conn_ptr);
	if(!conn_ptr)
	{
		fprintf(stderr,"init mysql failed!%s\n",mysql_error(conn_ptr));
		return -1;
	}
	conn_ptr = mysql_real_connect(conn_ptr,host,user,password,db,port,unix_socket,client_flag);
	if(!conn_ptr)
	{
		fprintf(stderr,"connection failed.error:%s\n",mysql_error(conn_ptr));
		return -1;
	}
	return 0;
}


int storeToMysql(Category category)
{
	if(connectToMysqlServer()<0) return -1;
	memset(sqlcmd,0,sizeof(sqlcmd));
	sprintf(sqlcmd,"insert into category values(%s,\"%s\",%s,%s,%s,%s)",category.thumbnail,category.id,category.cat,category.type,category.createtime,category.sort);
	//printf("%s\n",sqlcmd);
	if(mysql_query(conn_ptr,sqlcmd))
	{
		fprintf(stderr,"mysql_query failed.error:%s\n",mysql_error(conn_ptr));
	    mysql_close(conn_ptr);
		return -1;
	}
	mysql_close(conn_ptr);
	return 0;
}

int storeBlessToMysql(Bless bless)
{
	char escapeContent[4096];
	if(connectToMysqlServer()<0) return -1;
	memset(sqlcmd,0,sizeof(sqlcmd));
	mysql_escape_string(escapeContent,bless.content,strlen(bless.content));
	sprintf(sqlcmd,"insert into bless values(\"%s\",%s,\"%s\",\"%s\",%s)",bless.parentid,bless.parentname,bless.id,escapeContent,bless.icon_uri);
	printf("%s\n",sqlcmd);
	if(mysql_query(conn_ptr,sqlcmd))
	{
		fprintf(stderr,"mysql_query failed.error:%s\n",mysql_error(conn_ptr));
	    mysql_close(conn_ptr);
		return -1;
	}
	mysql_close(conn_ptr);
	return 0;
}



int selectIdFromCategory()
{
	MYSQL_RES* res;
	MYSQL_ROW row;
	if(connectToMysqlServer()<0) return -1;
	memset(sqlcmd,0,sizeof(sqlcmd));
 	sprintf(sqlcmd,"select * from category");
	if(mysql_query(conn_ptr,sqlcmd))
	{
		fprintf(stderr,"mysql_query failed.error:%s\n",mysql_error(conn_ptr));
	    mysql_close(conn_ptr);
		return -1;
	}
	res = mysql_use_result(conn_ptr);
	while((row=mysql_fetch_row(res)))
	{
		requestJsonStringWithId(row[1]);
		break;
	}
	mysql_free_result(res);
	mysql_close(conn_ptr);
	return 0;
}

