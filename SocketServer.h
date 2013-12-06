//
//  SocketServer.h
//  APIServer
//
//  Created by steven.zhan on 13-11-28.
//  Copyright (c) 2013年 Steven. All rights reserved.
//

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef APIServer_SocketServer_h
#define APIServer_SocketServer_h

int socketServerBegin();

#endif
