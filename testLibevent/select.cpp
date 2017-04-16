#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>

int doaccept(int fd)
{
	struct sockaddr_in cliAddr;
	socklen_t len = sizeof(cliAddr);
	bzero(&cliAddr, sizeof(cliAddr));

	int connfd = accept(fd, (struct sockaddr*)&cliAddr, &len);
	if(connfd < 0 || connfd > FD_SETSIZE)
		return -1;
	char ipaddr[32] = {0}; 
	inet_ntop(AF_INET,&(cliAddr.sin_addr),ipaddr,32);
	printf("max fd size = %d\n",FD_SETSIZE);
	printf("accept client connect %s:%d.\n", ipaddr,ntohs(cliAddr.sin_port));
	return connfd;
}

void doRead(int fd)
{
	int n;
	char buf[1024] = {0};
	while((n = recv(fd,buf, sizeof(buf),0))> 0)
	{
		printf("msg recv is %s\n", buf);
		char* temp = buf;
		while(*temp != '\r' && *temp != '\n')
			temp++;
		*temp = '\0';
		send(fd,buf,sizeof(buf),0);
		if(strcmp(buf,"quit") == 0)
			exit(0);
	}
}

int main()
{
	int listenfd;
	struct sockaddr_in serAddr;
	fd_set rdSet, allset;
	int fdArray[FD_SETSIZE] = {0};
	int maxfd;
	int flag;
	FD_ZERO(&rdSet);

	serAddr.sin_family =  AF_INET;
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serAddr.sin_port = htons(12345);

	for(int i =0; i< FD_SETSIZE; i++)
		fdArray[i] = -1;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0)
	{
		perror("create socket failed.\n");
		exit(-1);
	}
	if(flag = fcntl(listenfd,F_GETFL,0) < 0)
	{
		perror("FCNTL ERROR");
		return -1;
	}
	flag |= O_NONBLOCK;
	if(fcntl(listenfd, F_SETFL, flag) < 0 )
	{
		perror("fcntl set error");
		return -1;
	}
	printf("max fd size = %d\n",FD_SETSIZE);
     /*一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。*/
     int reuse = 1;
     if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
          exit(-1);
	if(bind(listenfd, (struct sockaddr*)&serAddr, sizeof(serAddr)) < 0)
	{
		perror("bind error");
		exit(-1);
	}

	if(listen(listenfd, 64) < 0)
	{
		perror("listen failed.\n");
		exit(-1);
	}
	FD_SET(listenfd, &rdSet);
	fdArray[0] = listenfd;
	maxfd = listenfd;
	while(1)
	{
		allset = rdSet;  //select每次检查前都必须重置描述符集
		int n = select(maxfd+1, &allset, NULL, NULL, NULL);
		if(n < 0)
		{
			perror("select failed.\n");
			exit(-1);
		}

		if(FD_ISSET(listenfd, &allset))
		{
			int connfd  = doaccept(listenfd);
			if(connfd < 0)
				exit(-1);
			if(flag = fcntl(connfd,F_GETFL,0) < 0)
			{
				perror("FCNTL ERROR");
				return -1;
			}
			flag |= O_NONBLOCK;
			//如果不将客户端套接字设置为非阻塞服务器将阻塞在某一个客户端套接字
			//的recv调用上，其他客户端的请求得不到响应
			if(fcntl(connfd, F_SETFL, flag) < 0 )  
			{
				perror("fcntl set error");
				return -1;
			}
			FD_SET(connfd, &rdSet);
			for(int i=0; i< FD_SETSIZE; i++)
			{
				if(fdArray[i] == -1)
				{
					fdArray[i] = connfd;
					break;
				}
			}
			//重置当前最大描述符字
			maxfd = (maxfd > connfd ? maxfd:connfd);

		}

		for(int i = 1; i< maxfd; i++)
		{
			if(FD_ISSET(fdArray[i], &allset))
				doRead(fdArray[i]);
		}
	
	}
	
	return 0;
}
