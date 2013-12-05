#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "getJsonString.h"
#include "parseJson.h"
#include "storeToMysql.h"



int main(int argc,char* argv[])
{
	char* retStr = getURLContentfromURLString("http://119.147.105.61:8080/app/v2/cat.jsp");
	parseJsonFromString(retStr);
	if(retStr) free(retStr);
	//storeToMysql(NULL);
	
	selectIdFromCategory();
	//connectToMysqlServer();
	return 0;
}
