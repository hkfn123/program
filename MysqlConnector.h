//
//  MysqlConnector.h
//  XMLParserExample
//
//  Created by Steven on 13-11-12.
//  Copyright (c) 2013年 Steven. All rights reserved.
//



#ifndef XMLParserExample_MysqlConnector_h
#define XMLParserExample_MysqlConnector_h

#endif

#define SUBJECT_LENGTH sizeof(subject)

typedef struct Subject
{
    char* total_count;
    char* id;
    struct Subject *next;
}subject;


void insertDataToTable(char* parent_id,char* parent_name,char* id,char* content,char *icon_uri);

void insertDataToSubjectTable(char* total_count,char* id,char* icon_uri,char* name,char *flag,char* updateFlag,char* isShow,char* orderNo,char* lastUpdateTime,char* contentLastUpdateTime);

void clearDataFromTable(char* tableName);

subject* selectIdAndTotal_Count(subject *headSubject);

subject* insertSubjectList(subject *pHead,char* id,char* total_count);

char* selectDatabaseWithSQLcom(char* sql);

long selectBlessWithIDStr(char* idstr);


