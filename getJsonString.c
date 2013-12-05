#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct MemoryStruct{
	char* memory;
	size_t size;
};


static size_t WriteMemoryCallback(void* contents,size_t size,size_t nmemb,void* userp)
{
	size_t realsize = size*nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	
	mem->memory = realloc(mem->memory,mem->size+realsize+1);
	if(mem->memory == NULL)
	{
		printf("not enough memory (realloc return NULL)\n");
		return 0;
	}
	memcpy(&(mem->memory[mem->size]),contents,realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	
	return realsize;
}


char* getURLContentfromURLString(const char* theURL)
{
	CURL* curl = NULL;
	CURLcode res;

 	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0;	
	
	if(chunk.memory == NULL)
	{
		fprintf(stderr,"not enough memory.\n");
		return NULL;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl == NULL)
	{
		fprintf(stderr,"curl init error!\n");
		return NULL;
	}
	fprintf(stderr,"cleaned!\n");
	curl_easy_setopt(curl,CURLOPT_URL,theURL);
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,3);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&chunk);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteMemoryCallback);	

	res = curl_easy_perform(curl);
	if(CURLE_OK != res){
		printf("curl_easy_perform() error:%s\n",curl_easy_strerror(res));
		return NULL;
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return chunk.memory;
}
