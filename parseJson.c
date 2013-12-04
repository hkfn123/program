#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json.h>
#include <error.h>
#include <malloc.h>

struct category{
	char thumbnail[255];
	char id[50];
	char cat[50];
	char type[50];
};


struct category* parseJsonFromString(const char* jsonStr)
{

	static struct category* aCategory=NULL;
	int i = 0;
	json_object* newJson_Object=NULL;
	json_object* thumbnail_Obj = NULL;
	json_object* id_Obj = NULL;
	json_object* cat_Obj = NULL;
	json_object* type_Obj = NULL;
	json_object* aJson_obj = NULL;
	json_object* bJson_obj = NULL;

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

	bJson_obj = json_object_object_get(aJson_obj,"DATA");
	if(!bJson_obj)
	{
		perror("json_object_object_get error!\n");
		return NULL;
	}

	bJson_obj = json_object_array_get_idx(bJson_obj,2);
	if(!aJson_obj)
	{
		perror("json_object_array_get() error!\n");
		return NULL;
	}
	
	bJson_obj = json_object_object_get(bJson_obj,"CATEGORY");
	if(!aJson_obj)
	{
		perror("json_object_object_get() error!\n");
		return NULL;
	}
	
	//printf("%s\n",json_object_to_json_string(bJson_obj));
	
 	int arr_len = json_object_array_length(bJson_obj);
	printf("%d\n",arr_len);

	for(i = 0;i < arr_len;i++)
	{
		newJson_Object = json_object_array_get_idx(bJson_obj,i);
		thumbnail_Obj = json_object_object_get(newJson_Object,"THUMBNAIL");
		id_Obj = json_object_object_get(newJson_Object,"ID");
		cat_Obj = json_object_object_get(newJson_Object,"CAT");
		type_Obj = json_object_object_get(newJson_Object,"TYPE");
		
		strcpy(aCategory->thumbnail,json_object_to_json_string(thumbnail_Obj));
		strcpy(aCategory->id,json_object_to_json_string(id_Obj));
		strcpy(aCategory->cat,json_object_to_json_string(cat_Obj));
		strcpy(aCategory->type,json_object_to_json_string(type_Obj));
		
		printf("%s\t%s\t%s\t%s\t\n",(*aCategory).thumbnail,(*aCategory).id,(*aCategory).cat,(*aCategory).type);

		json_object_put(thumbnail_Obj);
		json_object_put(id_Obj);
		json_object_put(cat_Obj);
		json_object_put(type_Obj);
	}

	json_object_put(aJson_obj);
	json_object_put(newJson_Object);

	return NULL;
}
