#ifndef __parseJson_h
#define __parseJson_h

typedef struct category{
	char createtime[50];
	char sort[10];
	char thumbnail[255];
	char id[50];
	char cat[50];
	char type[50];
}Category;

typedef struct bless{
	char parentid[50];
	char parentname[100];
	char id[50];
	char content[102400];
	char icon_uri[255];
}Bless;


Category* parseJsonFromString(const char* jsonStr);
int parseBlessFromJsonString(const char* jsonString);


#endif
