//
//  RequestXML.c
//  XMLParserExample
//
//  Created by Steven on 13-11-14.
//  Copyright (c) 2013年 Steven. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "RequestXML.h"
#include "xmlParser.h"

//http://cdn.ifanr.cn/wp-content/uploads/2011/03/xcode.png

size_t write_data(void* ptr,size_t size,size_t nmemb,FILE* stream);

void requestXMLWithUrl(char *theURL,int isMainPage)
{
    CURL *curl;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        res = curl_easy_setopt(curl, CURLOPT_URL,theURL);
    }
    
    FILE *fp = NULL;
    char* fileName = NULL;
    fileName = getFileNameWithURL(theURL);
    if ((fp = fopen(fileName, "wb"))==NULL) {
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,fp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1L);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        printf("Perform Error.\n%s",curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    fclose(fp);
    if (isMainPage) {
        parseMainPageXMLWithFileNameAndStoreToDatabase(getFileNameWithURL(theURL));
    }else{
        parseXMLWithFileNameAndStoreToDatabase(fileName,theURL);
    }
}

size_t write_data(void* ptr,size_t size,size_t nmemb,FILE* stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char* getFileNameWithURL(char *theurl)
{
    //return strrchr(theurl, '?')+1;
    return "bless.xml";
}