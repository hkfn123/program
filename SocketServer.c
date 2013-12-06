//
//  SocketServer.c
//  APIServer
//
//  Created by steven.zhan on 13-11-28.
//  Copyright (c) 2013年 Steven. All rights reserved.
//

#include <stdio.h>
#include "SocketServer.h"
#include <stdlib.h>
#include "MysqlConnector.h"

#define SERVER_PORT 8888
#define MAX_CONNECTION 1024


int socketServerBegin()
{
	int socket_fd,accept_fd;
	struct sockaddr_in server_address;
	size_t ret;
	//初始化socket
	if((socket_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket error!\r\n");
		return -1;
	}
	//初始化socket套接字
	bzero(&server_address,sizeof(struct sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	//绑定到本地端口
	if(bind(socket_fd,(struct sockaddr*)&server_address,sizeof(struct sockaddr)) < 0)
	{
		perror("bind error!\r\n");
		return -1;
	}
	//监听本地端口
	if(listen(socket_fd,MAX_CONNECTION) < 0)
	{
		perror("listen error!\r\n");
		return -1;
	}
	//接受客户端请求
	while(1)
	{
		accept_fd = accept(socket_fd,NULL,NULL);
		if((accept_fd<0) && (errno == EINTR))
			continue;
		else if(accept_fd < 0)
		{
			perror("accept error!\n");
			continue;
		}
		if(fork() == 0)
		{
			while(1)
			{
				char readBuffer[1024];
				bzero(readBuffer,1024);
				close(socket_fd);
				ret = read(accept_fd,readBuffer,1024);
				if(ret > 0)
				{
					char* writeBuffer = selectDatabaseWithSQLcom(readBuffer);
					if(NULL != writeBuffer)
					{
						write(accept_fd,writeBuffer,strlen(writeBuffer));
                        free(writeBuffer);
					}
				}
			}
		}
		close(accept_fd);
	}
	close(socket_fd);
}
