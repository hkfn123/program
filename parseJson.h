#ifndef __parseJson_h
#define __parseJson_h

typedef struct category{
	char thumbnail[255];
	char id[50];
	char cat[50];
	char type[50];
}Category;

Category* parseJsonFromString(const char* jsonStr);

#endif
