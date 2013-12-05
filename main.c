#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "getJsonString.h"
#include "parseJson.h"


int main(int argc,char* argv[])
{
	char* retStr = getURLContentfromURLString("http://www.wap8.cn/app/default_a.jsp?clienttype=android&uid=310260281126883");
	parseJsonFromString(retStr);
	if(retStr) free(retStr);
	//storeToMysql(NULL);
	
	return 0;
}
