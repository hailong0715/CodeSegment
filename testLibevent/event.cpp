#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <event2/event.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int make_socket_nonblock(int fd)
{
#ifdef _WIN32
	{
		unsigned long nonblocking = 1;
		if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
			return -1;
		}
	}
#else
	int flag;
	if((flag = fcntl(fd, F_GETFL,0)) < 0)
	{
		perror("FCNTL FGET");
		return -1;
	}
	flag |= O_NONBLOCK;
	if(fcntl(fd,F_SETFL, flag)< 0)
	{
		perror("FCNTL SET");
		return -1;
	}
#endif
	return 0;
}


int make_socket_reuseable(int fd)
{
#ifndef _WIN32
	int reuse = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		perror("set reuse addr");
		return -1;
	}
#endif
	return 0;
}

void doRead(evutil_socket_t fd, short event, void * arg)
{
	char recvBuf[1024] = {0};
	while(recv(fd,recvBuf, 1024, 0)>0)
	{
		send(fd,recvBuf, 1024, 0);
		char* temp = recvBuf;
		while(*temp != '\r' && *temp != '\n')
			temp++;
		*temp='\0';
		if(strcmp(recvBuf, "quit") == 0)
			exit(0);
	}
}


void doAccept(evutil_socket_t fd, short event, void * arg)
{
	int connfd;
	char ipAddr[32] = {0};
	struct event* readEvent = NULL;
	struct sockaddr_in cliAddr;
	bzero(&cliAddr, sizeof(cliAddr));
	socklen_t len = sizeof(cliAddr);

	struct event_base* base = (struct event_base*)arg;
	connfd = accept(fd, (struct sockaddr*)&cliAddr, &len);
	inet_ntop(AF_INET,&(cliAddr.sin_addr),ipAddr,32);
	printf("accept client connect %s:%d.\n", ipAddr,ntohs(cliAddr.sin_port));
	if(connfd < 0)
		return;
	make_socket_nonblock(connfd);
	//接受一个客户端连接后创建一个新的事件，并将事件添加到事件队列里，设置该事件的回调函数。
	readEvent = event_new(base, connfd, EV_READ|EV_PERSIST, doRead, NULL);
	if(!readEvent)
		return;
	event_add(readEvent, NULL);
}

int main()
{
	int listenfd, connfd;
	struct event_base* base;
	struct event *listenEvent;
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family =  AF_INET;
	serverAddr.sin_port = htons(12345);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//创建一个event_base结构
	base = event_base_new();
	if(!base)
	{
		perror("base new");
		return -1;
	}
		
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
	{
		perror("create socket");
		return -1;
	}

	//将套接字设置为非阻塞
	if(make_socket_nonblock(listenfd) < 0)
	{
		perror("make socket nonblock");
		return -1;
	}

	//将套接字地址设为可重用
	make_socket_reuseable(listenfd);
	
	if(bind(listenfd,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("bind error");
		return -1;
	}

	if(listen(listenfd, 64) < 0)
	{
		perror("listen fail");
		return -1;
	}
	//创建一个event,设置该事件为读事件，并设置EV_PERSIST属性，只有事件发生后该事件不从事件队列里删除
	//设置事件发生后的回调函数doAccept
	listenEvent = event_new(base, listenfd, EV_READ|EV_PERSIST, doAccept, (void*)base);
	if(!listenEvent)
	{
		perror("event new");
		return -1;
	}

	//将创建的事件添加到事件队列里去，类似于epoll的epoll_ctl add
	event_add(listenEvent, NULL);

	//开始事件循环，类似于epoll的epoll_wait
	event_base_dispatch(base);
}