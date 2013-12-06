//
//  aes_code.h
//  APIServer
//
//  Created by Steven on 13-11-28.
//  Copyright (c) 2013年 Steven. All rights reserved.
//

#include <stdio.h>
#include <openssl/aes.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>

#ifndef APIServer_aes_code_h
#define APIServer_aes_code_h

char* aes_enc(char *string);
char *aes_dec(char *pawd);


#endif
