
//
//  CommonMethod.c
//  APIServer
//
//  Created by steven.zhan on 13-11-16.
//  Copyright (c) 2013年 Steven. All rights reserved.
//

#include <stdio.h>
#include "CommonMethod.h"
#include <string.h>

int requestCount(long l)
{
    int temp = (l%20==0)?0:1;
    return (int)l/20+temp;
}

long charArrToLong(char *ch)
{
    
    long i;
    long temp = 0;
    for (i=strlen(ch); i>0; i--) {
        temp += (ch[i-1]-'0')*stevenpow(10,strlen(ch)-i);
    }
    return temp;
}

long stevenpow(long i,long j)
{
    long k;
    long temp = 1;
    for (k=0; k<j; k++) {
        temp *= i;
    }
    return temp;
}