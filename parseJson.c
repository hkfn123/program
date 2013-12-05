#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json.h>
#include <error.h>
#include <malloc.h>
#include "storeToMysql.h"

int parseBlessFromJsonString(const char* jsonString)
{
	Bless* aBless = NULL;
	int i = 0;
	json_object* aJson_obj = NULL;
	json_object* bJson_obj = NULL;
	json_object* newJson_Object = NULL;
	json_object* parent_id = NULL;
	json_object* parent_name = NULL;
	json_object* id = NULL;
	json_object* content = NULL;
	json_object* icon_uri = NULL;

	aBless = (Bless*)malloc(sizeof(Bless));
	if(!aBless)
	{
		fprintf(stderr,"malloc error!\n");
		return -1;
	}
	aJson_obj = json_tokener_parse(jsonString);
	if(!aJson_obj)
	{
		fprintf(stderr,"json_tokener_parse error!\n");
		return -1;
	}
	
	bJson_obj = json_object_object_get(aJson_obj,"data");
	
	int arr_len = json_object_array_length(bJson_obj);
	
	//printf("%d\n",arr_len);

	for(i = 0;i < arr_len;i++)
	{
		newJson_Object = json_object_array_get_idx(bJson_obj,i);

		parent_id = json_object_object_get(newJson_Object,"catid");
		parent_name = json_object_object_get(newJson_Object,"catname");
		id = json_object_object_get(newJson_Object,"id");
		content = json_object_object_get(newJson_Object,"content");
			
		strcpy(aBless->parentid,json_object_to_json_string(parent_id));
		strcat(aBless->parentid,"a");
		strcpy(aBless->id,json_object_to_json_string(id));
		strcat(aBless->id,"a");
		strcpy(aBless->parentname,json_object_to_json_string(parent_name));
		strcpy(aBless->content,json_object_to_json_string(content));
		strcpy(aBless->icon_uri,json_object_to_json_string(icon_uri));

		//printf("%s\t%s\t%s\t%s\t%s\t%s\t\n",(*aBless).parentid,(*aBless).parentname,(*aBless).id,(*aBless).content,aBless->icon_uri);
		
		storeBlessToMysql(*aBless);
		
		json_object_put(parent_id);
		json_object_put(parent_name);
		json_object_put(id);
		json_object_put(content);
		json_object_put(icon_uri);
		free(aBless);

	}
	json_object_put(aJson_obj);
	json_object_put(bJson_obj);
	json_object_put(newJson_Object);
	return 0;
}


Category* parseJsonFromString(const char* jsonStr)
{
	Category* aCategory=NULL;
	int i = 0;
	json_object* newJson_Object=NULL;
	json_object* thumbnail_Obj = NULL;
	json_object* id_Obj = NULL;
	json_object* cat_Obj = NULL;
	json_object* type_Obj = NULL;
	json_object* aJson_obj = NULL;
	json_object* bJson_obj = NULL;
	json_object* createtime_obj = NULL;
	json_object* sort_obj = NULL;

	aCategory = (struct category*)malloc(sizeof(struct category));
	if(aCategory==NULL)	
	{
		fprintf(stderr,"malloc error!\n");
		return NULL;
	}
	
	//parse json 
	aJson_obj = json_tokener_parse(jsonStr);	
	if(!aJson_obj)
	{
		fprintf(stderr,"json_tokener_parse error!\n");
		return NULL;
	}

	bJson_obj = json_object_object_get(aJson_obj,"data");
	if(!bJson_obj)
	{
		perror("json_object_object_get error!\n");
		return NULL;
	}

	//printf("%s\n",json_object_to_json_string(bJson_obj));
	
 	int arr_len = json_object_array_length(bJson_obj);
	
	//printf("%d\n",arr_len);

	for(i = 0;i < arr_len;i++)
	{
		newJson_Object = json_object_array_get_idx(bJson_obj,i);
		thumbnail_Obj = json_object_object_get(newJson_Object,"caticon");
		id_Obj = json_object_object_get(newJson_Object,"id");
		cat_Obj = json_object_object_get(newJson_Object,"name");
		type_Obj = json_object_object_get(newJson_Object,"type");
		createtime_obj = json_object_object_get(newJson_Object,"createtime");
		sort_obj  = json_object_object_get(newJson_Object,"sort");
		
		strcpy(aCategory->thumbnail,json_object_to_json_string(thumbnail_Obj));
		strcpy(aCategory->id,json_object_to_json_string(id_Obj));
		strcat(aCategory->id,"a");
		strcpy(aCategory->cat,json_object_to_json_string(cat_Obj));
		strcpy(aCategory->type,json_object_to_json_string(type_Obj));
		strcpy(aCategory->createtime,json_object_to_json_string(createtime_obj));
		strcpy(aCategory->sort,json_object_to_json_string(sort_obj));

		//printf("%s\t%s\t%s\t%s\t%s\t%s\t\n",(*aCategory).thumbnail,(*aCategory).id,(*aCategory).cat,(*aCategory).type,aCategory->createtime,aCategory->sort);
		storeToMysql(*aCategory);
		
		free(aCategory);
		json_object_put(thumbnail_Obj);
		json_object_put(id_Obj);
		json_object_put(cat_Obj);
		json_object_put(type_Obj);
		json_object_put(createtime_obj);
		json_object_put(sort_obj);
	}

	json_object_put(aJson_obj);
	json_object_put(newJson_Object);

	return NULL;
}
