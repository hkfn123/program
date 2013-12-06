//
//  xmlParser.c
//  XMLParserExample
//
//  Created by Steven on 13-11-12.
//  Copyright (c) 2013年 Steven. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include "MysqlConnector.h"
#include "RequestXML.h"
#include "CommonMethod.h"

void getBaseURL(char* theURL,char* baseURL);

int parseXMLWithFileNameAndStoreToDatabase(char* fileName,char* theURL)
{
    xmlDocPtr doc = NULL;
    xmlNodePtr curNode = NULL;
    xmlNodePtr sonNode = NULL;
    
    char* parent_name = NULL;
    char* parent_id = NULL;
    char* id = NULL;
    char* content = NULL;
    char* icon_uri = NULL;
    
    doc = xmlParseFile(fileName);
    if (NULL == doc) {
        printf("filename:%s\n",fileName);
        fprintf(stderr, "Document not parsed successfully\n");
        return -1;
    }
    
    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        fprintf(stderr, "empty Doucment\n");
        xmlFreeDoc(doc);
        return -1;
    }
    
    curNode = curNode->children;
    while (NULL != curNode) {
        if (!xmlStrcmp(curNode->name,(const xmlChar*)"bless")) {
            sonNode = curNode->children;
            while(NULL != sonNode) {
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"parent_name")){
                    parent_name = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"parent_id")){
                    parent_id = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"id")){
                    id = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"content")){
                    content = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"icon_uri")){
                    icon_uri = (char *)xmlNodeGetContent(sonNode);
                }
                sonNode = sonNode->next;
            }
        }
        insertDataToTable(parent_id, parent_name, id, content, icon_uri);
        curNode = curNode->next;
    }
    xmlFreeDoc(doc);
    if (id == NULL) {
        return 2;
    }else{
        long tempId = charArrToLong(id);
        char baseURL[512];
        memset(baseURL, 0, 512);
        getBaseURL(theURL,baseURL);
        sprintf(baseURL, "%s=%ld",baseURL,tempId);
        requestXMLWithUrl(baseURL,0);
    }

    return 0;
}

void getBaseURL(char* theURL,char* baseURL)
{
    char* b = strrchr(theURL, '=');
    long length = strlen(theURL)-strlen(b);
    int i = 0;
    for (i=0; i<length; i++) {
        baseURL[i] = *(theURL++);
    }
    printf("baseuri:%s\n",baseURL);
}

void parseMainPageXMLWithFileNameAndStoreToDatabase(char* fileName)
{
    clearDataFromTable("subject");
    
    xmlDocPtr doc = NULL;
    xmlNodePtr curNode = NULL;
    xmlNodePtr sonNode = NULL;
    
    char* total_count = NULL;
    char* id = NULL;
    char* icon_uri = NULL;
    char* name = NULL;
    char* flag = NULL;
    char* updateFlag = NULL;
    char* isShow = NULL;
    char* orderNo = NULL;
    char* lastUpdateTime = NULL;
    char* contentLastUpdateTime = NULL;
    
    doc = xmlParseFile(fileName);
    if (NULL == doc) {
        fprintf(stderr, "Document not parsed successfully\n");
        return;
    }
    
    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        fprintf(stderr, "empty Doucment\n");
        xmlFreeDoc(doc);
        return;
    }
    
    curNode = curNode->children;
    while (NULL != curNode) {
        if (!xmlStrcmp(curNode->name,(const xmlChar*)"subject")) {
            sonNode = curNode->children;
            while(NULL != sonNode) {
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"total_count")){
                    total_count = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"id")){
                    id = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"icon_uri")){
                    icon_uri = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"name")){
                    name = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"flag")){
                    flag = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"updateFlag")){
                    updateFlag = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"isShow")){
                    isShow = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"orderNo")){
                    orderNo = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"lastUpdateTime")){
                    lastUpdateTime = (char *)xmlNodeGetContent(sonNode);
                }
                if (!xmlStrcmp(sonNode->name, (const xmlChar*)"contentLastUpdateTime")){
                    contentLastUpdateTime = (char *)xmlNodeGetContent(sonNode);
                }
                sonNode = sonNode->next;
            }
        }
        insertDataToSubjectTable(total_count, id, icon_uri, name, flag, updateFlag, isShow, orderNo, lastUpdateTime, contentLastUpdateTime);
        curNode = curNode->next;
    }
    xmlFreeDoc(doc);
}

