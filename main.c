#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "getJsonString.h"
#include "parseJson.h"
#include "storeToMysql.h"
#include "RequestXML.h"
#include "MysqlConnector.h"
#include <time.h>
#include <unistd.h>
#include "SocketServer.h"
#include "aes_code.h"
#include "CommonMethod.h"

//http://appazf.aizhufu.cn:8087/t/findSubjectContentList.jhtml?subjectId=40&minId=0
#define MAIN_BASE_URL "http://appazf.aizhufu.cn:8087/t/findPushSubjectList/1.jhtml?lastUpdateTime="
#define BLESS_BASE_URL "http://appazf.aizhufu.cn:8087/t/findSubjectContentList.jhtml"

void requestJsonAndStoreToMysql();
void requestXMLAndStoreToMysql();
void startSocketServer();


void requestJsonAndStoreToMysql()
{
	char* retStr = getURLContentfromURLString("http://119.147.105.61:8080/app/v2/cat.jsp");
    parseJsonFromString(retStr);
    if(retStr) free(retStr);
    
    selectIdFromCategory();
}

void requestXMLAndStoreToMysql()
{
	char timeStr[20];
    sprintf(timeStr, "%ld", time(0));
    char mainUrl[512] = "";
    sprintf(mainUrl,"%s%s",MAIN_BASE_URL,timeStr);
	printf("%s",mainUrl);
    requestXMLWithUrl(mainUrl,1);
    requestXMLWithUrl("http://appazf.aizhufu.cn:8087/t/findSubjectContentList.jhtml?subjectId=40&minId=0",0);
    subject* subject = NULL;
    subject = selectIdAndTotal_Count(subject);
    
    char bless_origin_url[512];
    
    while (subject->next != NULL) {
        //printf("%s==%ld\n??",subject->total_count,charArrToLong(subject->total_count));
        //if ((selectBlessWithIDStr(subject->total_count)) == (charArrToLong(subject->total_count))) {
        //    continue;
       // }
        sprintf(bless_origin_url, "%s?subjectId=%s&minId=0\n",BLESS_BASE_URL,subject->id);
        requestXMLWithUrl(bless_origin_url,0);
        subject = subject->next;
    }
}

void startSocketServer()
{
	
}

int main(int argc,char* argv[])
{
	requestXMLAndStoreToMysql();
	//requestJsonAndStoreToMysql();
    return 0;
}
