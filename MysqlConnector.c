//
//  MysqlConnector.c
//  XMLParserExample
//
//  Created by Steven on 13-11-12.
//  Copyright (c) 2013年 Steven. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MysqlConnector.h"
#include <mysql.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "aes_code.h"

#define DATABASE_HOST "localhost"
#define DATABASE_USER "root"
#define DATABASE_PASSWORD "Foxconn123"
#define DATABASE_NAME "messageCollection"
#define DATABASE_PORT 3306


//连接数据库
MYSQL* connetToDatabase()
{
    MYSQL *mysql = NULL;
    
    if(!(mysql = mysql_init(mysql))){
        fprintf(stderr, "Mysql_init error.\n");
        return NULL;
    }
    
    if (!mysql_real_connect(mysql, DATABASE_HOST, DATABASE_USER, DATABASE_PASSWORD, DATABASE_NAME, DATABASE_PORT, "/var/lib/mysql/mysql.sock", 0)) {
        fprintf(stderr, "Can't connect to mysql server. Error:%s\n",mysql_error(mysql));
        return NULL;
    }
    return mysql;
}

//查询指定语句

char* selectDatabaseWithSQLcom(char* sql)
{
    int t;
    MYSQL *mysqlconn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
	xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root_list");
	xmlDocSetRootElement(doc,root_node);
    
    
    mysqlconn = connetToDatabase();
    
    if (!mysqlconn) {
        fprintf(stderr, "connect error.");
        return NULL;
    }
    
    char* desSql = aes_dec(sql);
    t = mysql_real_query(mysqlconn, desSql, strlen(desSql));
    if (t) {
        perror("select from database failed.\n");
        mysql_close(mysqlconn);
        return NULL;
    }
    
    res = mysql_store_result(mysqlconn);

    while ((row = mysql_fetch_row(res))) {
        xmlNodePtr object_node = xmlNewTextChild(root_node,NULL,BAD_CAST"object",NULL);
        for (t = 0; t < mysql_num_fields(res); t++) {
            if (mysql_num_fields(res) == 10) {
                switch (t) {
                    case 0:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"total_count",BAD_CAST(row[0]));
                        break;
                    case 1:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"id",BAD_CAST(row[1]));
                        break;
                    case 2:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"icon_uri",BAD_CAST(row[2]));
                        break;
                    case 3:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"name",BAD_CAST(row[3]));
                        break;
                    case 4:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"flag",BAD_CAST(row[4]));
                        break;
                    case 5:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"updateFlag",BAD_CAST(row[5]));
                        break;
                    case 6:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"isShow",BAD_CAST(row[6]));
                        break;
                    case 7:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"orderNo",BAD_CAST(row[7]));
                        break;
                    case 8:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"lastUpdateTime",BAD_CAST(row[8]));
                        break;
                    case 9:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"contentLastUpdateTime",BAD_CAST(row[9]));
                        break;    
                    default:
                        break;
                }
            }else if (mysql_num_fields(res) == 5){
                switch (t) {
                    case 0:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"parent_id",BAD_CAST(row[0]));
                        break;
                    case 1:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"parent_name",BAD_CAST(row[1]));
                        break;
                    case 2:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"id",BAD_CAST(row[2]));
                        break;
                    case 3:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"content",BAD_CAST(row[3]));
                        break;
                    case 4:
                        xmlNewTextChild(object_node,NULL,BAD_CAST"icon_uri",BAD_CAST(row[4]));
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    mysql_free_result(res);
    mysql_close(mysqlconn);
    free(desSql);
    
    xmlChar* xmlBuff;
    int buffersize = 1024000;
    xmlDocDumpFormatMemory(doc, &xmlBuff, &buffersize, 1);
    
    char* encryptxmlBuff = aes_enc((char*)xmlBuff);
    return encryptxmlBuff;
}


void insertDataToTable(char* parent_id,char* parent_name,char* id,char* content,char *icon_uri)
{
    MYSQL *mysql = NULL;
    char sqlcmd[1024];
    char esacpe_content[4096];
    int t;
    
    if (id == NULL) {
        return;
    }
    
    if ((mysql=connetToDatabase()) != NULL) {
        
        mysql_escape_string(esacpe_content, content, strlen(content));
        sprintf(sqlcmd, "insert into bless(parent_id,parent_name,id,content,icon_uri) values (\"%s\",\"%s\",\"%s\",\"%s\",\"%s\")",parent_id,parent_name,id,esacpe_content,icon_uri);
        t = mysql_query(mysql, sqlcmd);
        if (t) {
            printf("update database failed.Error:%s\n",mysql_error(mysql));
            mysql_close(mysql);
            return;
        }
    }
    mysql_close(mysql);
}

subject* selectIdAndTotal_Count(subject *headSubject){
    MYSQL *mysql = NULL;
    MYSQL_RES *res = NULL;
    char sqlcmd[512];
    MYSQL_ROW row;
    int t;
    if ((mysql=connetToDatabase()) != NULL) {
        sprintf(sqlcmd, "select id,total_count from subject where total_count != 0");
        t = mysql_real_query(mysql, sqlcmd, strlen(sqlcmd));
        if (t) {
            printf("select from subject error:%s\n",mysql_error(mysql));
            mysql_close(mysql);
            return headSubject;
        }else{
            res = mysql_store_result(mysql);
            while ((row = mysql_fetch_row(res))) {
                headSubject = insertSubjectList(headSubject,row[0], row[1]);
            }
        }
    }
    mysql_close(mysql);
    return headSubject;
}

subject* insertSubjectList(subject *pHead,char* id,char* total_count)
{
    subject *head = pHead;
    if (id == 0) {
        return pHead;
    }
    subject *sub = NULL;
    sub =(subject *)malloc(SUBJECT_LENGTH);
    if (sub == NULL ) {
        printf("malloc memory error.\n");
        exit(0);
    }
    memset(sub, 0, SUBJECT_LENGTH);
    sub->id = id;
    sub->total_count = total_count;
    if (pHead == NULL) {
        pHead = sub;
    }else{
        while (1) {
            if (head->next== NULL) {
                break;
            }else{
                head = head->next;
            }
        }
        head->next = sub;
        sub->next = NULL;
    }
    return pHead;
}

void insertDataToSubjectTable(char* total_count,char* id,char* icon_uri,char* name,char *flag,char* updateFlag,char* isShow,char* orderNo,char* lastUpdateTime,char* contentLastUpdateTime)
{
    clearDataFromTable("subject");
    
    MYSQL *mysql = NULL;
    char sqlcmd[512];
    int t;
    
    if (id == NULL) {
        return;
    }
    
   if ((mysql=connetToDatabase()) != NULL) { 
        sprintf(sqlcmd, "insert into subject values (\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\")",total_count,id,icon_uri,name,flag,updateFlag,isShow,orderNo,lastUpdateTime,contentLastUpdateTime);
        t = mysql_query(mysql, sqlcmd);
        if (t) {
            printf("update database failed.Error:%s\n",mysql_error(mysql));
            mysql_close(mysql);
            return;
        }
    }
    mysql_close(mysql);
}

void printSubjectList(subject* pHead)
{
	subject* head = pHead;
	while(head != NULL)
	{
		printf("%s\t%s\n",head->id,head->total_count);
		head=head->next;
	}
}

void clearDataFromTable(char* tableName)
{
    MYSQL *mysql = NULL;
    char sqlcmd[512];
    int t;
    if ((mysql=connetToDatabase()) != NULL) {
    sprintf(sqlcmd, "delete from subject");
    t = mysql_real_query(mysql, sqlcmd, strlen(sqlcmd));
    mysql_close(mysql);
    }
    mysql_close(mysql);
}

long selectBlessWithIDStr(char* idstr)
{
    MYSQL *mysql = NULL;
    char sqlcmd[512];
    MYSQL_RES* res = NULL;
    int t;
    
    if (idstr == NULL) {
        return 0;
    }
    
    if ((mysql=connetToDatabase()) != NULL) {
        sprintf(sqlcmd, "select * from bless where id=\"%s\"",idstr);
        printf("%s",sqlcmd);
        t = mysql_real_query(mysql, sqlcmd, strlen(sqlcmd));
        if (t) {
            printf("select from subject error:%s\n",mysql_error(mysql));
            mysql_close(mysql);
            return 0;
        }else{
            res = mysql_store_result(mysql);
            mysql_close(mysql);
            return mysql_num_rows(res);
        }
    }
    return 0;
}
