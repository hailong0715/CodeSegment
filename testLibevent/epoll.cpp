#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAX_EVENT_NUM 1024
int make_socket_nonblock(int fd)
{
	int flags;
	if((flags = fcntl(fd,F_GETFL,0))< 0 )
	{
		perror("get flags failed");
		return -1;
	}
	flags |= O_NONBLOCK;
	if(fcntl(fd, F_SETFL,flags) < 0)
	{
		perror("set flags failed");
		return -1;
	}
	return 0;
}


int make_socket_reuseable(int fd)
{
	int reuse = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
		return -1;
	return 0;
}

int doAccept(int fd)
{
	struct sockaddr_in cliAddr;
	char ipaddr[32] = {0}; 
	bzero(&cliAddr, sizeof(cliAddr));
	socklen_t len = sizeof(cliAddr);
	int connfd  = accept(fd, (struct sockaddr*)&cliAddr, &len);
	if(connfd < 0)
		return -1;
	inet_ntop(AF_INET,&(cliAddr.sin_addr),ipaddr,32);
	printf("accept client connect %s:%d.\n", ipaddr,ntohs(cliAddr.sin_port));
	return connfd;
}


void doRead(int fd)
{
	char recvBuf[1024] = {0};
	while(recv(fd,recvBuf, 1024, 0) > 0)
	{
		printf("msg recv is %s\n",recvBuf);
		send(fd, recvBuf, sizeof(recvBuf), 0);
		char * temp = recvBuf;
		while(*temp != '\r' && *temp != '\n')
			temp++;
		*temp == '\0';
		if(strcmp(recvBuf, "quit") == 0)
			exit(0);
	}
}

int main()
{
	int efd;
	int connfd;
	struct epoll_event event;
	struct epoll_event * events = NULL;
	struct sockaddr_in SerAddr;
	bzero(&SerAddr, sizeof(SerAddr));
	SerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	SerAddr.sin_port = htons(12345);
	SerAddr.sin_family = AF_INET;

	int listenfd = socket(AF_INET, SOCK_STREAM,0);
	if(listenfd < 0)
	{
		perror("listenfd < 0");
		return -1;
	}
	if(make_socket_nonblock(listenfd)< 0)
		return -1;
	if(make_socket_reuseable(listenfd) < 0)
		return -1;
	if(bind(listenfd, (struct sockaddr*)&SerAddr, sizeof(SerAddr))< 0)
	{
		perror("bind error");
		return -1;
	}
	efd = epoll_create(32000);
	if(efd < 0)
	{
		perror("epoll create");
		return -1;
	}
	event.data.fd = listenfd;
	event.events = EPOLLIN  | EPOLLET;
	if(epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &event) < 0)
	{
		perror("epoll-cntl");
		return -1;
	}
	events = (epoll_event*)calloc(MAX_EVENT_NUM, sizeof(event));
	while(1)
	{
		int n = epoll_wait(efd, events, MAX_EVENT_NUM, -1);
		for(int i=0; i < n; i++)
		{
			if(events[i].data.fd == listenfd)
			{
				if((connfd = doAccept(listenfd)) < 0)
				{
					perror("accept failed.\n");
					continue;
				}
				
				if(make_socket_nonblock(connfd) < 0)
				{
					perror("make non block failed");
					return -1;
				}
				
				event.data.fd = connfd;
				event.events = EPOLLIN | EPOLLET | EPOLLOUT ;
				if(epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &event) < 0)
				{
					perror("epoll add");
					return -1;
				}
			}
			else if(events[i].events & EPOLLIN)
			{
				doRead(listenfd);
			}
			else if(events[i].events & EPOLLOUT)
			{

			}
			else
			{
				perror("epoll error");
				close(events[i].data.fd);
			}
		}
	}
	return 0;
}
