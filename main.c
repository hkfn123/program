#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <arraylist.h>
#include "getJsonAndSaveToMysql.h"
#include <json.h>


int main(int argc,char* argv[])
{
	char* retStr = getURLContentfromURLString("http://www.wap8.cn/app/default_a.jsp?clienttype=android&uid=310260281126883");
	//printf("%s\n",retStr);
	
	json_object* json_obj = json_tokener_parse(retStr);
	if(!json_obj) printf("parse error!");
	//printf("%s\n",json_object_to_json_string(json_obj));

	json_object *pval = json_object_object_get(json_obj,"DATA");
	printf("%s\n",json_object_to_json_string(pval));
	json_object_put(pval);

	json_object_put(json_obj);
	
	if(retStr) free(retStr);

	
	return 0;
}
